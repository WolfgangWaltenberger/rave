#ifdef _WIN32
#define NOMINMAX
#endif
#include "BackPropagationCGAlgorithm.h"
#include "NeuronLayer.h"
#include "Neuron.h"
#include "InputNormaliserBuilder.h"
#include "InputNormaliserBuilderCatalogue.h"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <functional>
#include <valarray>

//#include <iostream>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

namespace NeuralNetUtils {

double BackPropCGDiff(double ele1,double ele2)
{
	return ((ele1-ele2)*(ele1-ele2));
}

template<typename T>
class MultValue
{
private:
    T _factor;
public:
    MultValue(const T& factor) :_factor(factor) {}
    ~MultValue(){};
    T operator()(T &elem) const
    {
        return elem*_factor;
    }
};

template<typename T>
class DivValue
{
private:
    T _factor;
public:
    DivValue(const T& factor) :_factor(factor) {}
    ~DivValue(){};
    T operator()(T &elem) const
    {
        return elem/_factor;
    }
};
}

BackPropagationCGAlgorithm::BackPropagationCGAlgorithm(NeuralNet &theNetwork)
: _theNetwork(theNetwork),_theBetaFunction(FletcherReves),
  _numberOfTrainingEvents(0),_runningEpochErrorTotal(0.0),
  _previousEpochError(0.0),_linearSearchStepLength(0.01),
  _linearSearchMu(0.001),_linearSearchSigma(0.1),
  _linearSearchMaxIterations(1000),
  _numberOfEpochs(0),_linearSearchGamma(0.05),
  _linearSearchTolerance(0.002),_progressPrintoutFrequency(100),
  _linearSearchAbsGradientCutoff(1.0E-9),
  _previousEpochStepLength(0.0)
{
    int numberOfNeurons = 0;
	for (int i=0;i<_theNetwork.numberOfLayers();++i)
	{
        numberOfNeurons += _theNetwork.layer(i)->numberOfNeurons();
		_neuronErrorSignals.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfNeurons()));
		_neuronOutputs.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfNeurons()));
		_neuronDerivativeOutputs.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfNeurons()));
		_dEdw.push_back(std::vector<double>(_theNetwork.layer(i)->numberOfWeights()));
	}
	_runningDeDwSum.assign(_theNetwork.numberOfWeights(),0.0);
	//_previousEpochDeDw.assign(_theNetwork.numberOfWeights(),0.0);
    //_previousEpochDeltaWeights.assign(_theNetwork.numberOfWeights(),0.0);
	_savedEpochErrorValues.clear();
    _epochsBeforeGradientReset = theNetwork.numberOfWeights()-numberOfNeurons;
}

BackPropagationCGAlgorithm::~BackPropagationCGAlgorithm(void)
{
}

void BackPropagationCGAlgorithm::setBetaFunction(BetaFunctionSelect theFunction)
{
	_theBetaFunction = theFunction;
}

double BackPropagationCGAlgorithm::error()
{
	std::vector<double> netOutput = _theNetwork.output(*_inputs);
	std::vector<double> differences(netOutput.size());
	std::transform(netOutput.begin(),netOutput.end(),_target->begin(),differences.begin(),
		NeuralNetUtils::BackPropCGDiff);
	double totalMeanSqError = std::accumulate(differences.begin(),differences.end(),0.0);
	return totalMeanSqError/2.0;
}

std::vector<double> BackPropagationCGAlgorithm::layerOutput(const int layer) const
{
	std::vector<double> inputs(*_inputs);
	if ((layer>=0)&&(layer<_theNetwork.numberOfLayers()))
	{
		for (int i=0;i<=layer;++i)
			inputs = _theNetwork.layer(i)->output(inputs);
	}
	return inputs;
}

void BackPropagationCGAlgorithm::calculateLayerOutputs()
{
	// Calculate the output from each neuron
	for (int layer=0;layer<_theNetwork.numberOfLayers();++layer)
	{
		std::vector<double> thelayerOutput(layerOutput(layer));
		_neuronOutputs[layer].assign(thelayerOutput.begin(),thelayerOutput.end());
	}
}

void BackPropagationCGAlgorithm::calculateDerivativeOutputs()
{
	// Calculate the derivative output from each neuron
	std::vector<double> thePreviousLayerOutput(*_inputs);
	for (int layer=0;layer<_theNetwork.numberOfLayers();++layer)
	{
		if (layer != 0) thePreviousLayerOutput = layerOutput(layer-1);

		std::vector<double> derivativeOutput = _theNetwork.layer(layer)->derivativeOutput(thePreviousLayerOutput);
		_neuronDerivativeOutputs[layer].assign(derivativeOutput.begin(),derivativeOutput.end());
	}
}

void BackPropagationCGAlgorithm::calculateErrorSignals()
{
	// Calculate initial error signals from network output
	std::vector<double> netOutput(_theNetwork.output(*_inputs));

	const int lastLayer = _theNetwork.numberOfLayers()-1;
	for (int i=0;i<(int)netOutput.size();++i)
	{
		double errorSignal = netOutput[i]-(*_target)[i]; // This is the -ve of the usual BP alg so we get grad not -grad

		_neuronErrorSignals[lastLayer][i] = errorSignal;
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

void BackPropagationCGAlgorithm::calculateDeDw()
{
	const std::vector<double> *inputs=_inputs;
	for (int layer=0;layer<_theNetwork.numberOfLayers();++layer)
	{
		_dEdw[layer].clear();
		if (layer>0) inputs = &_neuronOutputs[layer-1];
		std::vector<double> &derivative = _neuronDerivativeOutputs[layer];
		std::vector<double> &errorSignal = _neuronErrorSignals[layer];

		for (int node=0;node<_theNetwork.layer(layer)->numberOfNeurons();++node)
		{
			double bias = _theNetwork.layer(layer)->neuron(node)->bias();
			for (int i=0;i<(int)inputs->size();++i)
			{
				_dEdw[layer].push_back(errorSignal[node]*derivative[node]*((*inputs)[i]));
			}
			_dEdw[layer].push_back(errorSignal[node]*derivative[node]*bias);
		}
	}
}

double BackPropagationCGAlgorithm::beta(const std::vector<double> &gk,const std::vector<double> &gkplus1,
                                        const std::vector<double> &dk)
{
	if (_theBetaFunction == FletcherReves) return betaFR(gk,gkplus1);
	else if (_theBetaFunction == PolakRibiere) return betaPR(gk,gkplus1);
    else if (_theBetaFunction == ConjugateDescent) return betaCD(gk,gkplus1,dk);
	else return 0.0;
}

double BackPropagationCGAlgorithm::betaFR(const std::vector<double> &gk,const std::vector<double> &gkplus1)
{
	double tgk = std::inner_product(gk.begin(),gk.end(),gk.begin(),0.0);
	if (tgk != 0.0)
		return std::inner_product(gkplus1.begin(),gkplus1.end(),gkplus1.begin(),0.0)/tgk;
	else
		return 0.0;
}

double BackPropagationCGAlgorithm::betaCD(const std::vector<double> &gk,const std::vector<double> &gkplus1,
                                          const std::vector<double> &dk)
{
    double gkdk = std::inner_product(gk.begin(),gk.end(),dk.begin(),0.0);
    if (gkdk != 0.0)
        return -std::inner_product(gkplus1.begin(),gkplus1.end(),gkplus1.begin(),0.0)/gkdk;
    else
        return 0.0;
}

double BackPropagationCGAlgorithm::betaPR(const std::vector<double> &gk,const std::vector<double> &gkplus1)
{
	double tgk = std::inner_product(gk.begin(),gk.end(),gk.begin(),0.0);
	if (tgk != 0.0)
	{
		std::vector<double> gdiff(gk.size());
		std::transform(gkplus1.begin(),gkplus1.end(),gk.begin(),gdiff.begin(),std::minus<double>());
		double result = std::inner_product(gkplus1.begin(),gkplus1.end(),gdiff.begin(),0.0);
		return result/tgk;
	}
	else
		return 0.0;
}

double BackPropagationCGAlgorithm::alpha(const std::vector<double> &x,const std::vector<double> &p,
										 const std::vector<double> &g,const double F0,bool &converged)
{
	double steplength = _linearSearchStepLength;
	double phi0 = F0;
    double phi = F0;
    converged = false;

	std::vector<double> localg(g);
	std::vector<double> localx(x);
	std::vector<double> localp(p);
	std::vector<double> stepvector;

	double s0 = std::inner_product(localg.begin(),localg.end(),p.begin(),0.0);
	double modp = sqrt(std::inner_product(p.begin(),p.end(),p.begin(),0.0));

    if ((_previousEpochStepLength == 0.0)&&(_previousEpochError != 0.0))
    {
        if (modp != 0.0)
            steplength = std::min(std::abs((2.0*(_previousEpochError-phi0))/s0),1.0/modp);
    }
    else if ((_previousEpochError != 0.0) && (s0 != 0.0))
    {
        if (modp != 0.0)
            steplength = std::min(std::max(_previousEpochStepLength,std::abs((2.0*(_previousEpochError-phi0))/s0)),1.0/modp);
    }
    else if (_previousEpochError == 0.0)
    {
	    if (modp != 0.0)
		    steplength = std::max(_linearSearchStepLength,1.0/modp);
    }

#ifdef DEBUGLINESEARCH
	std::cout << "Initial step length = " << steplength << " modp = " << modp << " phi0 = " << phi0 << std::endl;
    std::cout << "last epoch step length = " << _previousEpochStepLength << std::endl;
    std::cout << "last epoch phi = " << _previousEpochError << std::endl;
    if (_previousEpochError != 0.0)
        std::cout << "Other candidate step length = " << std::abs((2.0*(_previousEpochError-phi0))/s0) << std::endl;
    std::cout << "s0 = " << s0 << std::endl;
#endif

	bool reverseSearch = false;
	if (s0 > 0.0)
	{
		reverseSearch = true;
		std::transform(p.begin(),p.end(),localp.begin(),std::negate<double>());
	}

    // Start of iteration loop
	int currentIteration = 0;
	while (currentIteration < _linearSearchMaxIterations)
	{

#ifdef DEBUGLINESEARCH
        std::cout << "<<<<< New Iteration <<<<<<<<<<<<<<" << std::endl;
        std::cout << "steplength = " << steplength << std::endl;
#endif

		bool rhcondition = false;
		bool lhcondition = false;
		double previousStepLength = steplength;
		
		double gkdotdk = std::inner_product(localg.begin(),localg.end(),localp.begin(),0.0);

		double rhs = phi0+_linearSearchMu*steplength*gkdotdk;

        // Wk+1 = Wk + step*dk
        stepvector.assign(localp.begin(),localp.end());
        std::transform(stepvector.begin(),stepvector.end(),stepvector.begin(),NeuralNetUtils::MultValue<double>(steplength));
		std::vector<double> newx(localx.size(),0.0);
		std::transform(localx.begin(),localx.end(),stepvector.begin(),newx.begin(),std::plus<double>());

#ifdef DEBUGLINESEARCH
        //for (int i=0;i<(int)localp.size()-1;++i)
        //    std::cout << localp[i] << ",";
        //std::cout << localp[(int)localp.size()-1] << std::endl;
        //for (int i=0;i<(int)stepvector.size()-1;++i)
        //    std::cout << stepvector[i] << ",";
        //std::cout << stepvector[(int)stepvector.size()-1] << std::endl;
        //for (int i=0;i<(int)newx.size()-1;++i)
        //    std::cout << newx[i] << ",";
        //std::cout << newx[(int)newx.size()-1] << std::endl;
#endif

		_theNetwork.setWeights(newx);
		_runningEpochErrorTotal = 0.0;
		_numberOfTrainingEvents = 0;
		_runningDeDwSum.assign(_runningDeDwSum.size(),0.0);
		processDataSet();

		phi0 = phi; // phi_k
		phi = _runningEpochErrorTotal/(double)_numberOfTrainingEvents; // phi_k+1

		if (phi <= rhs)
            rhcondition = true;
		
        double gkp1dotdk = std::inner_product(_runningDeDwSum.begin(),_runningDeDwSum.end(),localp.begin(),0.0);

#ifdef DEBUGLINESEARCH
		std::cout << "phi,gkdotdk,rhs " << phi << " , " << gkdotdk << " , " << rhs << std::endl;
		std::cout << "gkp1dotdk " << gkp1dotdk << std::endl;
		std::cout << "-_linearSearchSigma*gkdotdk " << -_linearSearchSigma*gkdotdk << std::endl;
#endif

		//// If new gradient is close to zero, say that we've converged and this is it!
		//if (std::abs(gkp1dotdk) < std::abs(_linearSearchAbsGradientCutoff))
		//{
		//	converged = true;
  //          lastEpochSteplength = steplength;
		//	if (reverseSearch)
		//		return -steplength;
		//	else
		//		return steplength;
		//}

		
		if ( std::abs(gkp1dotdk) <= -_linearSearchSigma*gkdotdk)
            lhcondition = true;

		if (lhcondition&&rhcondition)
		{

#ifdef DEBUGLINESEARCH
			std::cout << "Minimum found. Exiting with step length = " << steplength << std::endl;
#endif

			converged = true;
			if (reverseSearch)
				return -steplength;
			else
				return steplength;
		}
		else
		{
			if ((gkp1dotdk<0.0)&&(phi>=phi0))
			{
				steplength/=10.0;
				if (steplength < 1.0E-10) // Cutoff to quit if there is a problem
				{
#ifdef DEBUGLINESEARCH
					std::cerr << "Linear fit step length too small. Bailing out..." << std::endl;
#endif
                    double sl = std::max(-2.0*(phi-phi0)/s0,previousStepLength);
					if (reverseSearch)
						return -sl; 
					else
						return sl; 
				}
#ifdef DEBUGLINESEARCH
				std::cout << "new steplength " << steplength << std::endl;
#endif
			}
			else if ((gkp1dotdk<0.0)&&(phi<phi0))
			{
				steplength *= 10.0;
				localg = _runningDeDwSum;
				localx = newx;

#ifdef DEBUGLINESEARCH
				std::cout << "Increasing step length and resetting staring point" << std::endl;
				std::cout << "New step length = " << steplength << std::endl;
#endif
			}
			else if ((gkdotdk<0.0)&&(gkp1dotdk>0.0))
			{
#ifdef DEBUGLINESEARCH
				std::cout << "Cubic interpolation" << std::endl;
				std::cout << "gk " << gkdotdk << " gk+1 " << gkp1dotdk << " phi0 " << phi0 << " phi " << phi << " step " << steplength << std::endl;
#endif
				double D = phi0;
				double C = gkdotdk;
				double E = phi-D-C*steplength;
				double F = gkp1dotdk-C;
				double A = (steplength*F-2.0*E)/(steplength*steplength*steplength);
				double B = (3.0*E-steplength*F)/(steplength*steplength);
				double disc = B*B-3*A*C;
				double s1=0.0,s2=0.0;
				if ((disc<0.0)&&(disc>-_linearSearchTolerance))
					disc = 0.0;
				if (disc >= 0.0)
				{
					s1 = (-B+sqrt(disc))/(3.0*A);
					s2 = (-B-sqrt(disc))/(3.0*A);
				}
				else
				{
					s1 = steplength;
					s2 = -1.0;
				}

#ifdef DEBUGLINESEARCH
				std::cout << "s1,s2,disc = " << s1 << "," << s2 << "," << disc << std::endl;
#endif

				s1 = ((s1>0.0)&&(s1<steplength)) ? s1 : -1.0;
				s2 = ((s2>0.0)&&(s2<steplength)) ? s2 : -1.0;

#ifdef DEBUGLINESEARCH
				std::cout << "steplength,s1,s2 = " << steplength << "," << s1 << "," << s2 << std::endl;
                std::cout << "gamma,sl*gamma,(1-gamma)*sl " << _linearSearchGamma << "," << steplength*_linearSearchGamma << ","
                    << (1.0-_linearSearchGamma)*steplength << std::endl;
#endif

				if ((s1>0.0)&&(s2<0.0))
				{
					converged = true;
                    double sl = std::max(steplength*_linearSearchGamma,std::min(s1,(1.0-_linearSearchGamma)*steplength));
					if (reverseSearch)
						return -sl;
					else
						return sl;
				}
				else if ((s2>0.0)&&(s1<0.0))
				{
					converged = true;
                    double sl = std::max(steplength*_linearSearchGamma,std::min(s2,(1.0-_linearSearchGamma)*steplength));
					if (reverseSearch)
						return -sl;
					else
						return sl;
				}
				else if ((s1>0.0)&&(s2>0.0))
				{
					if ((A*s1*s1*s1+B*s1*s1+C*s1+D)<(A*s2*s2*s2+B*s2*s2+C*s2+D))
					{
						converged = true;
                        double sl = std::max(steplength*_linearSearchGamma,std::min(s1,(1.0-_linearSearchGamma)*steplength));
						if (reverseSearch)
							return -sl;
						else
							return sl;
					}
					else
					{
						converged = true;
						double sl = std::max(steplength*_linearSearchGamma,std::min(s2,(1.0-_linearSearchGamma)*steplength));
                        if (reverseSearch)
							return -sl;
						else
							return sl;
					}
				}
				else
				{
					// Cock-up in the interpolation with no roots in the allowed range.
#ifdef DEBUGLINESEARCH
					std::cout << "Cubic interpolation failed to find roots. Taking best guess" << std::endl;
#endif
                    double sl = std::max(-2.0*(phi0-phi)/s0,previousStepLength);
					if (reverseSearch)
						return -sl;
					else
						return sl;
				}
			}
		}
		currentIteration++;
	}
#ifdef DEBUGLINESEARCH
	std::cerr << "Linear search failed to converge." << std::endl;
#endif
	if (reverseSearch)
		return -steplength;
	else
		return steplength;
}


double BackPropagationCGAlgorithm::train(const int numberOfEpochs,const NeuralNetDataSet &dataSet,
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

double BackPropagationCGAlgorithm::train(const int numberOfEpochs,const NeuralNetDataSet &dataSet,
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

double BackPropagationCGAlgorithm::trainWithDataSet(const int numberOfEpochs)
{
	double epochError = 0.0;
    _numberOfEpochs = 0;

	_savedEpochErrorValues.clear();
    _currentSearchDirection.clear();

	for (int epoch=0;epoch<numberOfEpochs;++epoch)
	{
		//std::cout << "Epoch " << epoch << std::endl;
		double thisIterationError = processDataSet();
		bool goodEpoch = true;
        double gradient = 0.0;
		epochError = newEpoch(goodEpoch,gradient);
		if (!goodEpoch)
		{
			std::cerr << "Linear Search Failed. Bailing out..." << std::endl;
			break;
		}

        if (gradient < _linearSearchAbsGradientCutoff)
        {
            std::cout << "BackPropagationCGAlgorithm:: Gradient < cutoff" << std::endl << "(currently " << gradient << ", cutoff = "
                << _linearSearchAbsGradientCutoff << " )" << std::endl;
            std::cout << "Terminating minimisation - this probably means that the minimum"
                << std::endl << "has been found." << std::endl;
            std::cout << "Use setLinearSearchAbsGradientCutoff() to change the cutoff"
                << std::endl << "value if this is not the case." << std::endl;
            break;
        }
		_savedEpochErrorValues.push_back(epochError);
		if (_progressPrintoutFrequency > 0)
		{
			if (epoch%_progressPrintoutFrequency == 0)
				std::cout << "Epoch " << epoch << "/" << numberOfEpochs << " : Error function " << epochError << std::endl;
		}
	}
	_numberOfTrainingEvents = 0;
	_runningEpochErrorTotal = 0.0;
	_runningDeDwSum.assign(_theNetwork.numberOfWeights(),0.0);

    epochError = processDataSet()/(double)_numberOfTrainingEvents; // error of the final network
	_savedEpochErrorValues.push_back(epochError);
    std::cout << "Final error at end of training cycle : " << epochError << std::endl;
	return epochError;
}

void BackPropagationCGAlgorithm::calculateRunningDeDw()
{
		calculateLayerOutputs();
		calculateDerivativeOutputs();
		calculateErrorSignals();
		calculateDeDw();
		std::vector<double> tmp;
		for (int i=0;i<_theNetwork.numberOfLayers();++i)
		{
			std::vector<double> layerdedw(_dEdw[i]);
			tmp.insert(tmp.end(),layerdedw.begin(),layerdedw.end());
		}
		std::vector<double> tmp2(_theNetwork.numberOfWeights());
		std::transform(_runningDeDwSum.begin(),_runningDeDwSum.end(),tmp.begin(),tmp2.begin(),std::plus<double>());
		_runningDeDwSum.assign(tmp2.begin(),tmp2.end());
}

double BackPropagationCGAlgorithm::processDataSet()
{
	std::vector<double> inputs;
	std::vector<double> targets;
	double runningErrorBeforeThisIteration = _runningEpochErrorTotal;
	for (int i=0;i<_currentDataSet->numberOfDataItems();++i)
	{
		_currentDataSet->getDataItem(i,inputs,targets);

		_inputs = &inputs;
		_target = &targets;
		calculateRunningDeDw();
		_runningEpochErrorTotal += error();
		_numberOfTrainingEvents++;
	}
    std::transform(_runningDeDwSum.begin(),_runningDeDwSum.end(),_runningDeDwSum.begin(),
                    NeuralNetUtils::DivValue<double>((double)_numberOfTrainingEvents));
	return _runningEpochErrorTotal-runningErrorBeforeThisIteration;
}


double BackPropagationCGAlgorithm::newEpoch(bool &success,double &gradient)
{

#ifdef DEBUGLINESEARCH
    std::cout << std::endl << "<<<<<<<<<<<< New Epoch <<<<<<<<" << std::endl;
#endif

	double tmpError = _runningEpochErrorTotal/(double)_numberOfTrainingEvents;
	_previousEpochDeDw.assign(_runningDeDwSum.begin(),_runningDeDwSum.end()); // grad(phi)

	if (_currentSearchDirection.empty())
    {
        // Set direction to be -grad(error func)
        _currentSearchDirection.assign(_runningDeDwSum.begin(),_runningDeDwSum.end());
        std::transform(_currentSearchDirection.begin(),_currentSearchDirection.end(),
            _currentSearchDirection.begin(),std::negate<double>());
    }


    std::vector<double> currentWeights(_theNetwork.weights());
	double a = alpha(currentWeights,_currentSearchDirection,_previousEpochDeDw,
        _runningEpochErrorTotal/(double)_numberOfTrainingEvents,success);

#ifdef DEBUGLINESEARCH
	std::cout << "a = " << a << std::endl;
#endif

	if (!success)
		return -1.0;

    _previousEpochError = tmpError;
    _previousEpochStepLength = a;

	std::vector<double> newWeights(currentWeights.size(),0.0);
    
    // New weights Wk+1 = Wk + a*Dk
    std::vector<double> step(_currentSearchDirection);
    std::transform(step.begin(),step.end(),step.begin(),NeuralNetUtils::MultValue<double>(a));
	std::transform(currentWeights.begin(),currentWeights.end(),step.begin(),newWeights.begin(),std::plus<double>());

	_theNetwork.setWeights(newWeights);
	_numberOfTrainingEvents = 0;
	_runningEpochErrorTotal = 0.0;
	_runningDeDwSum.assign(_runningDeDwSum.size(),0.0);
	processDataSet();
    _runningEpochErrorTotal /= (double)_numberOfTrainingEvents;

	double b = beta(_previousEpochDeDw,_runningDeDwSum,_currentSearchDirection);

#ifdef DEBUGLINESEARCH
    std::cout << "b = " << b << std::endl;
#endif

	std::vector<double> delta(_currentSearchDirection);
    std::transform(delta.begin(),delta.end(),delta.begin(),NeuralNetUtils::MultValue<double>(b)); // Dk*b

    std::vector<double> newdirection(_runningDeDwSum);
    std::transform(newdirection.begin(),newdirection.end(),newdirection.begin(),std::negate<double>()); // -grad(phi(Wk+1))

    gradient = sqrt(std::inner_product(newdirection.begin(),newdirection.end(),newdirection.begin(),0.0));

#ifdef DEBUGLINESEARCH
    std::cout << "Gradient = " << gradient << std::endl;
#endif

	if (_numberOfEpochs == _epochsBeforeGradientReset)
	{
#ifdef DEBUGLINESEARCH
        std::cout << "Resetting search direction to steepest descent" << std::endl;
        std::cout << "Momentum constant = " << std::max(0.0,std::min(1.0,a*b)) << std::endl;
#endif
		_numberOfEpochs = 0;
        _currentSearchDirection.assign(newdirection.begin(),newdirection.end()); // Reset to steepest descent
        _previousEpochStepLength = 0.0;
        if (!_previousEpochDeltaWeights.empty())
        {
            // Since we're resetting to steepest descent, try to add a momentum term to get us out of
            // local minima
            double momentumConstant = std::max(0.0,std::min(1.0,a*b));
            std::transform(_previousEpochDeltaWeights.begin(),_previousEpochDeltaWeights.end(),_previousEpochDeltaWeights.begin(),
                NeuralNetUtils::MultValue<double>(momentumConstant));
            std::transform(newWeights.begin(),newWeights.end(),_previousEpochDeltaWeights.begin(),newWeights.begin(),
                    std::plus<double>());
            _theNetwork.setWeights(newWeights);
        }
	}
	else
    {
        // New search direction Dk+1 = -grad(phi(Wk+1)) + b*Dk
	    std::transform(newdirection.begin(),newdirection.end(),delta.begin(),_currentSearchDirection.begin(),std::plus<double>());
        
        // If the search direction is not in a descent direction, reset to steepest descent 
        double tmp = std::inner_product(_currentSearchDirection.begin(),_currentSearchDirection.end(),newdirection.begin(),0.0);
        if (tmp < 0.0)
        {
#ifdef DEBUGLINESEARCH
            std::cout << "Resetting search direction to steepest descent" << std::endl;
            std::cout << "Momentum constant = " << std::max(0.0,std::min(1.0,a*b)) << std::endl;
#endif
            _currentSearchDirection.assign(newdirection.begin(),newdirection.end()); // Reset to steepest descent
            _previousEpochStepLength = 0.0;
            if (!_previousEpochDeltaWeights.empty())
            {
                // Since we're resetting to steepest descent, try to add a momentum term to get us out of
                // local minima
                double momentumConstant = std::max(0.0,std::min(1.0,a*b));
                std::transform(_previousEpochDeltaWeights.begin(),_previousEpochDeltaWeights.end(),_previousEpochDeltaWeights.begin(),
                    NeuralNetUtils::MultValue<double>(momentumConstant));
                std::transform(newWeights.begin(),newWeights.end(),_previousEpochDeltaWeights.begin(),newWeights.begin(),
                    std::plus<double>());
                _theNetwork.setWeights(newWeights);
            }
       }

		++_numberOfEpochs;
    }

    _previousEpochDeltaWeights.assign(step.begin(),step.end());

	_numberOfTrainingEvents = 0;
	_runningEpochErrorTotal = 0.0;
	_runningDeDwSum.assign(_theNetwork.numberOfWeights(),0.0);
	return _previousEpochError;
}
