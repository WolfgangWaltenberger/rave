#include "LinearNeuronBuilder.h"
#include "LinearNeuron.h"

#include <valarray>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

LinearNeuronBuilder::LinearNeuronBuilder(void)
: _slopeEnd(0.5)
{
}

LinearNeuronBuilder::~LinearNeuronBuilder(void)
{
}

Neuron *LinearNeuronBuilder::buildNeuron(const int numberOfInputs,const double bias) const
{
	return new LinearNeuron(numberOfInputs,_slopeEnd,bias,_theNetwork);
}

Neuron *LinearNeuronBuilder::buildNeuron(const int numberOfInputs,const std::vector<double> &constructionData) const
{
	if ((int)constructionData.size() >= numberOfInputs+3)
	{
		std::vector<double> weights(constructionData.begin(),
			constructionData.begin()+numberOfInputs+1);
		Neuron *theNeuron = new LinearNeuron(numberOfInputs,
			constructionData[numberOfInputs+2],constructionData[numberOfInputs+1],_theNetwork);
		theNeuron->setWeights(weights);
		return theNeuron;
	}
	else
		return (Neuron *)NULL;
}

void LinearNeuronBuilder::setConstructedNeuronSlopeEnd(const double slopeEnd)
{
	_slopeEnd = std::abs(slopeEnd);
}
