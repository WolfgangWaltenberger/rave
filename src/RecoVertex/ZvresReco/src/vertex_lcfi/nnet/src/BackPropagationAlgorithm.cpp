#include "BackPropagationAlgorithm.h"

#include "NeuronLayer.h"
#include "Neuron.h"

#include <cmath>
#include <algorithm>
#include <numeric>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

namespace NeuralNetUtils {

double BackPropDiff(double ele1,double ele2)
{
	return ((ele1-ele2)*(ele1-ele2));
}
}

BackPropagationAlgorithm::BackPropagationAlgorithm(NeuralNet &theNetwork,const double learningRate,const double momentumConstant)
: _theNetwork(theNetwork),_learningRate(learningRate),_momentumConstant(momentumConstant)
{
	for (int i=0;i<_theNetwork.numberOfLayers();++i)
	{
		_neuronErrorSignals.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfNeurons()));
		_neuronOutputs.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfNeurons()));
		_neuronDerivativeOutputs.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfNeurons()));
	}
	_momentumWeights = std::vector<double>(_theNetwork.numberOfWeights(),0.0);
}

BackPropagationAlgorithm::~BackPropagationAlgorithm(void)
{
}

double BackPropagationAlgorithm::train(const std::vector<double> &inputValues,const std::vector<double> &desiredOutput)
{
	_inputs = &inputValues;
	_target = &desiredOutput;
	calculateLayerOutputs();
	calculateDerivativeOutputs();
	calculateErrorSignals();
	calculateDeltaWeights();
	return error();
}

std::vector<double> BackPropagationAlgorithm::layerOutput(const int layer) const
{
	std::vector<double> inputs = *_inputs;
	if ((layer>=0)&&(layer<_theNetwork.numberOfLayers()))
	{
		for (int i=0;i<=layer;++i)
			inputs = _theNetwork.layer(i)->output(inputs);
	}
	return inputs;
}

void BackPropagationAlgorithm::calculateLayerOutputs()
{
	// Calculate the output from each neuron
	for (int layer=0;layer<_theNetwork.numberOfLayers();++layer)
	{
		std::vector<double> thelayerOutput = layerOutput(layer);
		_neuronOutputs[layer].assign(thelayerOutput.begin(),thelayerOutput.end());
	}
}

void BackPropagationAlgorithm::calculateDerivativeOutputs()
{
	// Calculate the derivative output from each neuron
	std::vector<double> thePreviousLayerOutput;
	for (int layer=0;layer<_theNetwork.numberOfLayers();++layer)
	{
		if (layer ==0) thePreviousLayerOutput = *_inputs;
		else thePreviousLayerOutput = layerOutput(layer-1);

		std::vector<double> derivativeOutput = _theNetwork.layer(layer)->derivativeOutput(thePreviousLayerOutput);
		_neuronDerivativeOutputs[layer].assign(derivativeOutput.begin(),derivativeOutput.end());
	}
}

void BackPropagationAlgorithm::calculateErrorSignals()
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

void BackPropagationAlgorithm::calculateDeltaWeights()
{
	std::vector<double> deltaWeights;
	const std::vector<double> *inputs=_inputs;
	for (int layer=0;layer<_theNetwork.numberOfLayers();++layer)
	{
		if (layer>0) inputs = &_neuronOutputs[layer-1];
		std::vector<double> &derivative = _neuronDerivativeOutputs[layer];
		std::vector<double> &errorSignal = _neuronErrorSignals[layer];

		for (int node=0;node<_theNetwork.layer(layer)->numberOfNeurons();++node)
		{
			double bias = _theNetwork.layer(layer)->neuron(node)->bias();
			for (int i=0;i<(int)inputs->size();++i)
			{
				deltaWeights.push_back(_learningRate*errorSignal[node]*derivative[node]*((*inputs)[i]));
			}
			deltaWeights.push_back(_learningRate*errorSignal[node]*derivative[node]*bias);
		}
	}
	std::vector<double> networkWeights = _theNetwork.weights();
	for (int i=0;i<(int)networkWeights.size();++i)
	{
		deltaWeights[i]+=_momentumConstant*_momentumWeights[i];
		networkWeights[i]+=deltaWeights[i];
	}
	_theNetwork.setWeights(networkWeights);
	_momentumWeights.assign(deltaWeights.begin(),deltaWeights.end());
}

double BackPropagationAlgorithm::error()
{
	std::vector<double> netOutput = _theNetwork.output(*_inputs);
	std::vector<double> differences(netOutput.size());
	std::transform(netOutput.begin(),netOutput.end(),_target->begin(),differences.begin(),
		NeuralNetUtils::BackPropDiff);
	double totalMeanSqError = std::accumulate(differences.begin(),differences.end(),0.0);
	return totalMeanSqError/2.0;
}
