#ifndef SIGMOIDNEURON_H
#define SIGMOIDNEURON_H

#include "NeuralNetConfig.h"
#include "Neuron.h"

#include <iostream>
#include <string>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class NeuralNet;

class NEURALNETDLL SigmoidNeuron :
	public Neuron
{
public:
	SigmoidNeuron(const int numberOfInputs,const double bias=-1.0,double response=1.0,const NeuralNet *parent=0);
	double response() const {return _response;}
	void setResponse(double response) {_response = response;}
	void destroy() const;
	Neuron *clone(const NeuralNet *parentNetwork) const;
	void outputRange(double &outputmin,double &outputmax) const
	{ outputmin = 0.0; outputmax = 1.0;}


protected:
	~SigmoidNeuron(void);
	double thresholdFunction(const double activation) const;
	double derivative(const double x) const;
	void serialiseExtra(std::ostream &os) const;
	std::string neuronType() const {return "SigmoidNeuron";}

private:
    SigmoidNeuron(const SigmoidNeuron &other); // Declared but not defined
	double _response;
};

}//namespace nnet

#endif
