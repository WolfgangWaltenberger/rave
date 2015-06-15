#ifndef TANSIGMOIDNEURON_H
#define TANSIGMOIDNEURON_H

#include "NeuralNetConfig.h"
#include "Neuron.h"

#ifdef __CINT__
#include "NeuralNet.h"
#else
//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{
class NeuralNet;
}
#endif

#include <iostream>
#include <string>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
TanSigmoidNeuron :
	public Neuron
{
public:
	TanSigmoidNeuron(const int numberOfInputs,const double bias=-1.0,const double scale=1.0,const NeuralNet *parent=0);
	~TanSigmoidNeuron(void);
	void setScale(const double newScale) {_scale = newScale;}
	double scale() {return _scale;}
	void destroy() const;
	Neuron *clone(const NeuralNet *parentNetwork) const;
	void outputRange(double &outputmin,double &outputmax) const
	{ outputmin = -1.0; outputmax = 1.0;}

protected:
	double thresholdFunction(const double activation) const;
	double derivative(const double x) const;
	void serialiseExtra(std::ostream &os) const;
	std::string neuronType() const {return "TanSigmoidNeuron";}

private:
    TanSigmoidNeuron(const TanSigmoidNeuron &other); // Declared but not defined
	double _scale;
};

}//namespace nnet

#endif
