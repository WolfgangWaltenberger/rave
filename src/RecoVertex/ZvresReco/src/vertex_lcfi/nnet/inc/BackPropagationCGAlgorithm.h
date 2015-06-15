#ifndef BACKPROPAGATIONCGALGORITHM_H
#define BACKPROPAGATIONCGALGORITHM_H

#include "NeuralNetConfig.h"

#include "NeuralNet.h"
#include "NeuralNetDataSet.h"

#include <vector>

#ifdef __CINT__
#include "InputNormaliser.h"
#else
//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{
class InputNormaliser;
}
#endif

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
BackPropagationCGAlgorithm
{
public:
	typedef enum { FletcherReves, PolakRibiere, ConjugateDescent } BetaFunctionSelect;

public:
	BackPropagationCGAlgorithm(NeuralNet &theNetwork);
	~BackPropagationCGAlgorithm(void);
	double train(const int numberOfEpochs,const NeuralNetDataSet &dataSet,
        const NeuralNet::InputNormalisationSelect normaliseTrainingData=NeuralNet::PassthroughNormalised);
    double train(const int numberOfEpochs,const NeuralNetDataSet &dataSet,const std::vector<InputNormaliser *> &inputNormalisers);
	void setBetaFunction(BetaFunctionSelect theFunction);
	void setLinearSearchInitialStepLength(const double stepLength) {_linearSearchStepLength = stepLength;}
	void setLinearSearchMu(const double mu) {_linearSearchMu = mu;}
	void setLinearSearchSigma(const double sigma) {_linearSearchSigma = sigma;}
	void setLinearSearchGamma(const double gamma) {_linearSearchGamma = gamma;}
	void setLinearSearchMaxIterations(const int maxIterations) {_linearSearchMaxIterations = maxIterations;}
	void setLinearSearchAbsGradientCutoff(const double cutoff) {_linearSearchAbsGradientCutoff = cutoff;}
	void setEpochsBeforeGradientReset(const int numberOfEpochs) {_epochsBeforeGradientReset = numberOfEpochs;}
	void setProgressPrintoutFrequency(const int frequency) {_progressPrintoutFrequency = frequency;}
	std::vector<double> getTrainingErrorValuesPerEpoch() const {return _savedEpochErrorValues;}

protected:
    double trainWithDataSet(const int numberOfEpochs);
	std::vector<double> layerOutput(const int layer) const;
	void calculateLayerOutputs();
	void calculateDerivativeOutputs();
	void calculateErrorSignals();
	void calculateDeDw();
	void calculateRunningDeDw();
	double error();
	double newEpoch(bool &success,double &gradient);
	double processDataSet();
    double beta(const std::vector<double> &gk,const std::vector<double> &gkplus1,const std::vector<double> &dk);
	double betaFR(const std::vector<double> &gk,const std::vector<double> &gkplus1);
	double betaPR(const std::vector<double> &gk,const std::vector<double> &gkplus1);
    double betaCD(const std::vector<double> &gk,const std::vector<double> &gkplus1,const std::vector<double> &dk);
	double alpha(const std::vector<double> &x,const std::vector<double> &p,const std::vector<double> &g,const double F0,bool &converged);

private:
	typedef std::vector<std::vector<double> > NetMatrix;

private:
	NeuralNet &_theNetwork;
	BetaFunctionSelect _theBetaFunction;
	const std::vector<double> *_inputs,*_target;
	NetMatrix _neuronErrorSignals;
	NetMatrix _neuronOutputs;
	NetMatrix _neuronDerivativeOutputs;
	NetMatrix _dEdw;
	const NeuralNetDataSet *_currentDataSet;
	double _previousEpochError;
	double _runningEpochErrorTotal;
	int _numberOfTrainingEvents;
	std::vector<double> _runningDeDwSum;
	std::vector<double> _previousEpochDeDw;
	double _linearSearchStepLength;
	std::vector<double> _currentSearchDirection;
	double _linearSearchMu;
	double _linearSearchSigma;
	double _linearSearchGamma;
	double _linearSearchTolerance;
	int _linearSearchMaxIterations;
	int _epochsBeforeGradientReset;
	int _numberOfEpochs;
	int _progressPrintoutFrequency;
	std::vector<double> _savedEpochErrorValues;
	double _linearSearchAbsGradientCutoff;
    double _previousEpochStepLength;
    std::vector<double> _previousEpochDeltaWeights;
};

}//namespace nnet

#endif
