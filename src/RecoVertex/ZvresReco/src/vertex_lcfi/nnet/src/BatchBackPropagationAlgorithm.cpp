#include "BatchBackPropagationAlgorithm.h"

#include "NeuronLayer.h"
#include "Neuron.h"
#include "InputNormaliserBuilder.h"
#include "InputNormaliserBuilderCatalogue.h"

#include <algorithm>
#include <numeric>
#include <iostream>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

namespace NeuralNetUtils {

double BatchBackPropDiff(double ele1,double ele2)
{
	return ((ele1-ele2)*(ele1-ele2));
}
}

BatchBackPropagationAlgorithm::BatchBackPropagationAlgorithm(NeuralNet &theNetwork,const double learningRate,const double momentumConstant)
: _theNetwork(theNetwork),_learningRate(learningRate),_momentumConstant(momentumConstant),_maxErrorInc(1.00004),_numberOfTrainingEvents(0),
  _previousEpochError(1000000.0),_runningEpochErrorTotal(0.0),_progressPrintoutFrequency(100),_epochsToWaitBeforeRestore(5)
{
	for (int i=0;i<_theNetwork.numberOfLayers();++i)
	{
		_neuronErrorSignals.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfNeurons()));
		_neuronOutputs.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfNeurons()));
		_neuronDerivativeOutputs.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfNeurons()));
		_runningGradientTotal.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfWeights()));
	}
	_momentumWeights = std::vector<double>(_theNetwork.numberOfWeights(),0.0);
	_previousEpochWeights = _theNetwork.weights();
	_savedEpochErrorValues.clear();
}

BatchBackPropagationAlgorithm::~BatchBackPropagationAlgorithm(void)
{
}

double BatchBackPropagationAlgorithm::train(const int numberOfEpochs,const NeuralNetDataSet &dataSet,
                                            const NeuralNet::InputNormalisationSelect normaliseTrainingData)
{
	_currentDataSet = &dataSet;

 	std::vector<double> inputmean;
	std::vector<double> targetoffset;
	std::vector<double> inputvariance;
	std::vector<double> targetrange;
    std::vector<double> inputoffset;
    std::vector<double> inputrange;
	dataSet.getNormalisationData(inputmean,targetoffset,inputvariance,targetrange,inputoffset,inputrange);

    if (normaliseTrainingData==NeuralNet::GaussianNormalised)
	{

		if (((int)inputmean.size() != _theNetwork.numberOfInputs())||((int)inputvariance.size() != _theNetwork.numberOfInputs()))
			std::cerr << "Normalisation error: input size mismatch" << std::endl;

        InputNormaliserBuilder *theBuilder = InputNormaliserBuilderCatalogue::instance(&_theNetwork)->builderOf("GaussianNormaliser");
        std::vector<InputNormaliser *> theNormalisers;
        for (int i=0;i<_theNetwork.numberOfInputs();++i)
        {
            std::vector<double> constructionData;
            constructionData.clear();
            constructionData.push_back(inputmean[i]);
            constructionData.push_back(inputvariance[i]);
            theNormalisers.push_back(theBuilder->buildNormaliser(constructionData));
        }
        _theNetwork.setInputNormalisers(theNormalisers);
	}

	std::vector<std::pair<double,double> > netoutranges = _theNetwork.networkOutputRange();
	std::vector<double> netoffsets;
	std::vector<double> netranges;
	for (int i=0;i<(int)targetoffset.size();++i)
	{
		double targetRange = netoutranges[i].second-netoutranges[i].first;
		netoffsets.push_back( -((targetrange[i]/targetRange)*netoutranges[i].first+targetoffset[i]));
		netranges.push_back( targetrange[i]/targetRange );
	}
	_theNetwork.setTargetNormalisationOffsets(netoffsets);
	_theNetwork.setTargetNormalisationRanges(netranges);

    return trainWithDataSet(numberOfEpochs);
}

double BatchBackPropagationAlgorithm::train(const int numberOfEpochs,const NeuralNetDataSet &dataSet,
                                         const std::vector<InputNormaliser *> &inputNormalisers)
{
    _currentDataSet = &dataSet;

    _theNetwork.setInputNormalisers(inputNormalisers);

    std::vector<double> inputmean;
	std::vector<double> targetoffset;
	std::vector<double> inputvariance;
	std::vector<double> targetrange;
    std::vector<double> inputoffset;
    std::vector<double> inputrange;
	dataSet.getNormalisationData(inputmean,targetoffset,inputvariance,targetrange,inputoffset,inputrange);

	std::vector<std::pair<double,double> > netoutranges = _theNetwork.networkOutputRange();
	std::vector<double> netoffsets;
	std::vector<double> netranges;

	for (int i=0;i<(int)targetoffset.size();++i)
	{
		double targetRange = netoutranges[i].second-netoutranges[i].first;
		netoffsets.push_back( -((targetrange[i]/targetRange)*netoutranges[i].first+targetoffset[i]));
		netranges.push_back( targetrange[i]/targetRange );
	}
	_theNetwork.setTargetNormalisationOffsets(netoffsets);
	_theNetwork.setTargetNormalisationRanges(netranges);

    return trainWithDataSet(numberOfEpochs);
}

double BatchBackPropagationAlgorithm::trainWithDataSet(const int numberOfEpochs)
{
	double epochError = 0.0;
	double tmpMomentumConstant = 0.0;
	bool momentumConstantReset = false;
	int epochToRestoreMomentumConstant = 0;
    _numberOfTrainingEvents = 0;

    _savedEpochErrorValues.clear();

	for (int epoch=0;epoch<numberOfEpochs;++epoch)
	{
		double thisIterationError = processDataSet();
		if ((_runningEpochErrorTotal/(double)_numberOfTrainingEvents) > _maxErrorInc*_previousEpochError)
		{
			std::cout << "Error increased. Trying to recover.." << std::endl;
			std::cout << "Previous error " << _previousEpochError << " current error " << _runningEpochErrorTotal/(double)_numberOfTrainingEvents << std::endl;
			for (int i=0;i<_theNetwork.numberOfLayers();++i)
			{
				_runningGradientTotal[i].assign(_runningGradientTotal[i].size(),0.0);
			}
			_theNetwork.setWeights(_previousEpochWeights);
			_numberOfTrainingEvents = 0;
			_runningEpochErrorTotal = 0.0;
			if (!momentumConstantReset)
				tmpMomentumConstant = _momentumConstant;
			_momentumConstant = 0.0;
			momentumConstantReset = true;
			epochToRestoreMomentumConstant = epoch+_epochsToWaitBeforeRestore;
			thisIterationError = processDataSet();
		}
		else
		{
			_previousEpochWeights = _theNetwork.weights();
		}
		epochError = newEpoch();
		_savedEpochErrorValues.push_back(epochError);
		if (momentumConstantReset)
		{
			if (epoch == epochToRestoreMomentumConstant)
				_momentumConstant = tmpMomentumConstant;
			momentumConstantReset = false;
		}
		if (_progressPrintoutFrequency > 0)
		{
			if (epoch%_progressPrintoutFrequency == 0)
				std::cout << "Epoch " << epoch << "/" << numberOfEpochs << " : Error function " << epochError << std::endl;
		}
	}

	_numberOfTrainingEvents = 0;
	_runningEpochErrorTotal = 0.0;
    epochError = processDataSet()/(double)_numberOfTrainingEvents; // error of the final network
	_savedEpochErrorValues.push_back(epochError);
    std::cout << "Final error at end of training cycle : " << epochError << std::endl;

	return epochError;
}

double BatchBackPropagationAlgorithm::processDataSet()
{
	std::vector<double> inputs;
	std::vector<double> targets;
	double runningErrorBeforeThisIteration = _runningEpochErrorTotal;
	for (int i=0;i<_currentDataSet->numberOfDataItems();++i)
	{
		_currentDataSet->getDataItem(i,inputs,targets);

		_inputs = &inputs;
		_target = &targets;
		calculateLayerOutputs();
		calculateDerivativeOutputs();
		calculateErrorSignals();
		calculateRunningGradientTotal();
		_runningEpochErrorTotal += error();
		_numberOfTrainingEvents++;
	}
	return _runningEpochErrorTotal-runningErrorBeforeThisIteration;
}

std::vector<double> BatchBackPropagationAlgorithm::layerOutput(const int layer) const
{
	std::vector<double> inputs = *_inputs;
	if ((layer>=0)&&(layer<_theNetwork.numberOfLayers()))
	{
		for (int i=0;i<=layer;++i)
			inputs = _theNetwork.layer(i)->output(inputs);
	}
	return inputs;
}

void BatchBackPropagationAlgorithm::calculateLayerOutputs()
{
	// Calculate the output from each neuron
	for (int layer=0;layer<_theNetwork.numberOfLayers();++layer)
	{
		std::vector<double> thelayerOutput = layerOutput(layer);
		_neuronOutputs[layer].assign(thelayerOutput.begin(),thelayerOutput.end());

		//std::cout << "Layer " << layer << " output" << std::endl;
		//for (std::vector<double>::iterator iter = _neuronOutputs[layer].begin();
		//	iter != _neuronOutputs[layer].end();++iter)
		//	std::cout << *iter << " ";
		//std::cout << std::endl;
	}
}

void BatchBackPropagationAlgorithm::calculateDerivativeOutputs()
{
	// Calculate the derivative output from each neuron
	std::vector<double> thePreviousLayerOutput;
	for (int layer=0;layer<_theNetwork.numberOfLayers();++layer)
	{
		if (layer ==0) thePreviousLayerOutput = *_inputs;
		else thePreviousLayerOutput = layerOutput(layer-1);

		std::vector<double> derivativeOutput = _theNetwork.layer(layer)->derivativeOutput(thePreviousLayerOutput);
		_neuronDerivativeOutputs[layer].assign(derivativeOutput.begin(),derivativeOutput.end());

		//std::cout << "Layer " << layer << " derivative output" << std::endl;
		//for (std::vector<double>::iterator iter = _neuronDerivativeOutputs[layer].begin();
		//	iter != _neuronDerivativeOutputs[layer].end();++iter)
		//	std::cout << *iter << " ";
		//std::cout << std::endl;
	}
}

void BatchBackPropagationAlgorithm::calculateErrorSignals()
{
	// Calculate initial error signals from network output
	std::vector<double> netOutput = _theNetwork.output(*_inputs);

	for (int i=0;i<(int)netOutput.size();++i)
	{
		double errorSignal = (*_target)[i]-netOutput[i];

		_neuronErrorSignals[_theNetwork.numberOfLayers()-1][i] = errorSignal;
	}
	for (int layer=_theNetwork.numberOfLayers()-2;layer>=0;--layer)
	{
		for (int node=0;node<_theNetwork.layer(layer)->numberOfNeurons();++node)
		{
			double errorSignal = 0.0;
			for (int nextlayernode=0;nextlayernode<_theNetwork.layer(layer+1)->numberOfNeurons();++nextlayernode)
			{
				std::vector<double> nodeWeights = _theNetwork.layer(layer+1)->neuron(nextlayernode)->weights();
				double nodeDerivative = _neuronDerivativeOutputs[layer+1][nextlayernode];
				double nodeErrorSignal = _neuronErrorSignals[layer+1][nextlayernode];
				errorSignal += nodeErrorSignal*nodeDerivative*nodeWeights[node];
			}
			_neuronErrorSignals[layer][node] = errorSignal;
		}
	}
}

void BatchBackPropagationAlgorithm::calculateRunningGradientTotal()
{
	const std::vector<double> *inputs=_inputs;
	for (int layer=0;layer<_theNetwork.numberOfLayers();++layer)
	{
		if (layer>0) inputs = &_neuronOutputs[layer-1];
		std::vector<double> &derivative = _neuronDerivativeOutputs[layer];
		std::vector<double> &errorSignal = _neuronErrorSignals[layer];
		int currentLayerWeight = 0;

		for (int node=0;node<_theNetwork.layer(layer)->numberOfNeurons();++node)
		{
			double bias = _theNetwork.layer(layer)->neuron(node)->bias();
			for (int i=0;i<(int)inputs->size();++i)
			{
				_runningGradientTotal[layer][currentLayerWeight] += errorSignal[node]*derivative[node]*((*inputs)[i]);
				currentLayerWeight++;
			}
			_runningGradientTotal[layer][currentLayerWeight] += errorSignal[node]*derivative[node]*bias;
			++currentLayerWeight;
		}
	}
}

void BatchBackPropagationAlgorithm::calculateDeltaWeights()
{
	std::vector<double> deltaWeights;
	for (int layer=0;layer<_theNetwork.numberOfLayers();++layer)
	{
		for (int i=0;i<(int)_runningGradientTotal[layer].size();++i)
		{
			double normalisedGradient = _runningGradientTotal[layer][i]/(double)_numberOfTrainingEvents;
			double deltaWeight = (_learningRate*normalisedGradient)+(_momentumConstant*_momentumWeights[i]);
			deltaWeights.push_back(deltaWeight);
		}
	}
	std::vector<double> networkWeights = _theNetwork.weights();
	for (int i=0;i<(int)networkWeights.size();++i)
	{
		networkWeights[i]+=deltaWeights[i];
	}
	_theNetwork.setWeights(networkWeights);
	_momentumWeights.assign(deltaWeights.begin(),deltaWeights.end());
}

double BatchBackPropagationAlgorithm::error()
{
	std::vector<double> netOutput = _theNetwork.output(*_inputs);
	std::vector<double> differences(netOutput.size());
	std::transform(netOutput.begin(),netOutput.end(),_target->begin(),differences.begin(),
		NeuralNetUtils::BatchBackPropDiff);
	double totalMeanSqError = std::accumulate(differences.begin(),differences.end(),0.0);
	return totalMeanSqError/2.0;
}

double BatchBackPropagationAlgorithm::newEpoch()
{
	calculateDeltaWeights();
	for (int i=0;i<_theNetwork.numberOfLayers();++i)
	{
		_runningGradientTotal[i].assign(_runningGradientTotal[i].size(),0.0);
	}
	_previousEpochError = _runningEpochErrorTotal/(double)_numberOfTrainingEvents;
	_numberOfTrainingEvents = 0;
	_runningEpochErrorTotal = 0.0;
	return _previousEpochError;
}
