#include "Neuron.h"
#include "NeuralNet.h"
#include "RandomNumberUtils.h"

#include <cstdlib>
#include <algorithm>
#include <numeric>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

Neuron::Neuron(const int numberOfInputs,const double bias,const NeuralNet *parentNetwork)
: _numberOfInputs(numberOfInputs),
  _bias(bias),
  _weights(numberOfInputs+1),
  _parentNetwork(parentNetwork)
{
	std::generate(_weights.begin(),_weights.end(),NeuralNetRandom::RandomGauss);
}

Neuron::~Neuron(void)
{
}

double Neuron::output(const std::vector<double> &inputValues) const
{
	return thresholdFunction(activation(inputValues)); 
}

double Neuron::derivativeOutput(const std::vector<double> &inputValues) const
{
	return derivative(activation(inputValues));
}

void Neuron::setWeights(const std::vector<double> &newWeights)
{
    std::vector<double>::const_iterator weightIter = newWeights.begin();
    _weights.assign(weightIter,weightIter+numberOfWeights());
}

double Neuron::activation(const std::vector<double> &inputs) const
{
    double result = std::inner_product(inputs.begin(),inputs.end(),_weights.begin(),0.0);
	result += _bias*_weights[_numberOfInputs];
	return result;
}

void Neuron::serialise(std::ostream &os) const
{
    if (_parentNetwork != (NeuralNet *)0)
    {
        NeuralNet::SerialisationMode mode = _parentNetwork->getSerialisationMode();
        if (mode == NeuralNet::PlainText)
        {
            os << neuronType() << " " << _numberOfInputs << " ";
            for (int i=0;i<_numberOfInputs+1;++i)
	            os << _weights[i] << " ";
            os << _bias << " ";
        }
        else if (mode == NeuralNet::XML)
        {
	        os << "<Neuron Type=\"" << neuronType() << "\" NumberOfInputs=\"" << _numberOfInputs << "\">" << std::endl;
	        for (int i=0;i<_numberOfInputs+1;++i)
		        os << "<weight>" << _weights[i] << "</weight>" << std::endl;
	        os << "<bias>" << _bias << "</bias>" << std::endl;
        }

	    serialiseExtra(os);

        if (mode == NeuralNet::PlainText)
	        os << std::endl;
        else if (mode == NeuralNet::XML)
	        os << "</Neuron>" << std::endl;
    }
}
