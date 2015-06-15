#include "TanSigmoidNeuron.h"
#include "NeuralNet.h"

#include <valarray>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

TanSigmoidNeuron::TanSigmoidNeuron(const int numberOfInputs,const double bias,const double scale,const NeuralNet *parent)
: Neuron(numberOfInputs,bias,parent),_scale(scale)
{
}

TanSigmoidNeuron::~TanSigmoidNeuron(void)
{
}

void TanSigmoidNeuron::destroy() const
{
	delete this;
}

Neuron *TanSigmoidNeuron::clone(const NeuralNet *parentNetwork) const
{
	Neuron *theNewNeuron = new TanSigmoidNeuron(_numberOfInputs,_bias,_scale,parentNetwork);
	theNewNeuron->setWeights(weights());
	return theNewNeuron;
}

double TanSigmoidNeuron::thresholdFunction(const double activation) const
{
	return std::tanh(_scale*activation);
}

double TanSigmoidNeuron::derivative(const double x) const
{
	double tmp = std::tanh(_scale*x);
	return 1.0-(tmp*tmp);
}

void TanSigmoidNeuron::serialiseExtra(std::ostream &os) const
{
    if (_parentNetwork != (NeuralNet *)0)
    {
        NeuralNet::SerialisationMode mode = _parentNetwork->getSerialisationMode();
        if (mode == NeuralNet::XML)
            os << "<scale>" << _scale << "</scale>" << std::endl;
        else if (mode == NeuralNet::PlainText)
	        os << _scale;
    }
}
