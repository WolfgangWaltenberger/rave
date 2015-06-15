#include "SigmoidNeuronBuilder.h"

#include "SigmoidNeuron.h"

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

SigmoidNeuronBuilder::SigmoidNeuronBuilder(void)
: _response(1.0)
{
}

SigmoidNeuronBuilder::~SigmoidNeuronBuilder(void)
{
}

Neuron *SigmoidNeuronBuilder::buildNeuron(const int numberOfInputs,const double bias) const
{
	return new SigmoidNeuron(numberOfInputs,bias,_response,_theNetwork);
}

Neuron *SigmoidNeuronBuilder::buildNeuron(const int numberOfInputs,const std::vector<double> &constructionData) const
{
	if ((int)constructionData.size() >= numberOfInputs+3)
	{
		std::vector<double> weights(constructionData.begin(),
			constructionData.begin()+numberOfInputs+1);
		Neuron *theNeuron = new SigmoidNeuron(numberOfInputs,
			constructionData[numberOfInputs+1],constructionData[numberOfInputs+2],_theNetwork);
		theNeuron->setWeights(weights);
		return theNeuron;
	}
	else
		return (Neuron *)NULL;
}

