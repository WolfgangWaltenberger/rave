#include "LinearNeuron.h"
#include "NeuralNet.h"

#include <valarray>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

LinearNeuron::LinearNeuron(const int numberOfInputs,const double slopeEnd,const double bias,const NeuralNet *parent)
: Neuron(numberOfInputs,bias,parent)
{
	_slopeEnd = std::abs(slopeEnd);
}

LinearNeuron::~LinearNeuron(void)
{
}

void LinearNeuron::destroy() const
{
	delete this;
}

void LinearNeuron::setSlopeEnd(const double slopeEnd)
{
	_slopeEnd = std::abs(slopeEnd);
}

double LinearNeuron::thresholdFunction(const double activation) const
{
	if (_slopeEnd != 0.0)
	{
		if (activation<= -_slopeEnd)
			return -1.0;
		else if (activation>=_slopeEnd)
			return 1.0;
		else
			return (activation/_slopeEnd);

	}
	else
	{
		if (activation != 0.0)
			return activation<0.0 ? -1.0 : 1.0;
		else
			return 0.0;
	}
}

double LinearNeuron::derivative(const double x) const
{
	if (_slopeEnd != 0.0)
	{
		if ((x<= -_slopeEnd)||(x>=_slopeEnd))
			return 0.0;
		else
			return 1.0/_slopeEnd;
	}
	else
		return 0.0;
}

void LinearNeuron::serialiseExtra(std::ostream &os) const
{
    if (_parentNetwork != (NeuralNet *)0)
    {
        NeuralNet::SerialisationMode mode = _parentNetwork->getSerialisationMode();
        if (mode == NeuralNet::XML)
            os << "<slopeEnd>" << _slopeEnd << "</slopeEnd>" << std::endl;
        else if (mode == NeuralNet::PlainText)
            os << _slopeEnd;
    }
}

Neuron *LinearNeuron::clone(const NeuralNet *parentNetwork) const
{
	Neuron *theNewNeuron = new LinearNeuron(_numberOfInputs,_slopeEnd,_bias,parentNetwork);
	theNewNeuron->setWeights(weights());
	return theNewNeuron;
}
