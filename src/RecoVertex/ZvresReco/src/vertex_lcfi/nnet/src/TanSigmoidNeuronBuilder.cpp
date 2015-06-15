#include "TanSigmoidNeuronBuilder.h"

#include "TanSigmoidNeuron.h"

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

TanSigmoidNeuronBuilder::TanSigmoidNeuronBuilder(void)
: _scale(1.0)
{
}

TanSigmoidNeuronBuilder::~TanSigmoidNeuronBuilder(void)
{
}

Neuron *TanSigmoidNeuronBuilder::buildNeuron(const int numberOfInputs,const double bias) const
{
	return new TanSigmoidNeuron(numberOfInputs,bias,_scale,_theNetwork);
}

Neuron *TanSigmoidNeuronBuilder::buildNeuron(const int numberOfInputs,const std::vector<double> &constructionData) const
{
	if ((int)constructionData.size() >= numberOfInputs+3)
	{
		std::vector<double> weights(constructionData.begin(),
			constructionData.begin()+numberOfInputs+1);
		Neuron *theNeuron = new TanSigmoidNeuron(numberOfInputs,
			constructionData[numberOfInputs+1],constructionData[numberOfInputs+2],_theNetwork);
		theNeuron->setWeights(weights);
		return theNeuron;
	}
	else
		return (Neuron *)NULL;
}
