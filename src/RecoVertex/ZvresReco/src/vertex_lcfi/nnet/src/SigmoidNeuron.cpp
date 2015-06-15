#include "SigmoidNeuron.h"
#include "NeuralNet.h"

#include <cmath>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

SigmoidNeuron::SigmoidNeuron(const int numberOfInputs,const double bias,double response,const NeuralNet *parent)
: Neuron(numberOfInputs,bias,parent), _response(response)
{
}

SigmoidNeuron::~SigmoidNeuron(void)
{
}

void SigmoidNeuron::destroy() const
{
	delete this;
}

double SigmoidNeuron::thresholdFunction(const double activation) const
{
	if (_response != 0.0)
		return 1.0/(1.0+exp(-activation/_response));
	else
		if (activation != 0.0)
			return activation<0.0 ? 0.0 : 1.0 ;
		else
			return 0.5;
}

double SigmoidNeuron::derivative(const double x) const
{
	if (_response != 0.0)
	{
		double sigmoid = thresholdFunction(x);
		return (1.0/_response)*sigmoid*(1.0-sigmoid);
	}
	else
	{
		return 0.0;
	}

}

void SigmoidNeuron::serialiseExtra(std::ostream &os) const
{
    if (_parentNetwork != (NeuralNet *)0)
    {
        NeuralNet::SerialisationMode mode = _parentNetwork->getSerialisationMode();
        if (mode == NeuralNet::XML)
	        os << "<response>" << _response << "</response>" << std::endl;
        else if (mode == NeuralNet::PlainText)
	        os << _response;
    }
}

Neuron *SigmoidNeuron::clone(const NeuralNet *parentNetwork) const
{
	Neuron *theNewNeuron = new SigmoidNeuron(_numberOfInputs,_bias,_response,parentNetwork);
	theNewNeuron->setWeights(weights());
	return theNewNeuron;
}
