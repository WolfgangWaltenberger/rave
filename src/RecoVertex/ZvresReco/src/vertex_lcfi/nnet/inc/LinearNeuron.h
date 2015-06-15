#ifndef LINEARNEURON_H
#define LINEARNEURON_H

#include "NeuralNetConfig.h"
#include "Neuron.h"

#include <iostream>
#include <string>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class NeuralNet;

class NEURALNETDLL LinearNeuron :
	public Neuron
{
public:
	LinearNeuron(const int numberOfInputs,const double slopeEnd=0.5,const double bias=-1.0,const NeuralNet *parent=0);
	void destroy() const;
	double slopeEnd() const {return _slopeEnd;}
	void setSlopeEnd(const double slopeEnd);
	Neuron *clone(const NeuralNet *parentNetwork) const;
	void outputRange(double &outputmin,double &outputmax) const
	{ outputmin = -1.0; outputmax = 1.0;}

protected:
	~LinearNeuron(void);
	double thresholdFunction(const double activation) const;
	double derivative(const double x) const;
	void serialiseExtra(std::ostream &os) const;
	std::string neuronType() const {return "LinearNeuron";}

private:
    LinearNeuron(const LinearNeuron &other); // Declared but not defined
	double _slopeEnd;
};

}//namespace nnet

#endif
