#ifndef NEURON_H
#define NEURON_H

#include "NeuralNetConfig.h"

#include <vector>
#include <iostream>
#include <string>

#ifdef __CINT__
#include "NeuralNet.h"
#else
//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{
class NeuralNet;
}
#endif

// A simple Neuron class that defines an
// interface to concrete neurons.
// Note that neurons can only be created on
// the heap, i.e. you have to new them.

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
Neuron
{
public:
	double output(const std::vector<double> &inputValues) const;
	double derivativeOutput(const std::vector<double> &inputValues) const;
	std::vector<double> weights() const {return _weights;}
	double bias() const {return _bias;}
	int numberOfWeights() {return (int)_weights.size();}
	void setWeights(const std::vector<double> &newWeights);
	void serialise(std::ostream &os) const;
	virtual void destroy() const = 0;
	virtual Neuron *clone(const NeuralNet *parentNetwork) const = 0;
	virtual void outputRange(double &outputmin,double &outputmax) const = 0;

protected:
	Neuron(const int numberOfInputs,const double bias,const NeuralNet *parentNetwork=0);
	virtual ~Neuron(void);
	virtual double thresholdFunction(const double activation) const = 0;
	virtual double derivative(const double x) const = 0;
	virtual void serialiseExtra(std::ostream &os) const {};
	virtual std::string neuronType() const = 0;

private:
	double activation(const std::vector<double> &inputs) const;

protected:
	const int _numberOfInputs;
	const double _bias;
	std::vector<double> _weights;
    const NeuralNet *_parentNetwork;
};

}//namespace nnet

#endif
