#ifndef LINEARNEURONBUILDER_H
#define LINEARNEURONBUILDER_H

#include "NeuralNetConfig.h"
#include "NeuronBuilder.h"

#include <string>
#include <vector>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class Neuron;

class NEURALNETDLL LinearNeuronBuilder
	: public NeuronBuilder
{
public:
	LinearNeuronBuilder(void);
	~LinearNeuronBuilder(void);
	Neuron *buildNeuron(const int numberOfInputs,const double bias=-1.0) const;
	Neuron *buildNeuron(const int numberOfInputs,const std::vector<double> &constructionData) const;
	void setConstructedNeuronSlopeEnd(const double slopeEnd);
	std::string buildsType() const
		{ return "LinearNeuron";}

private:
	double _slopeEnd;
};

}//namespace nnet

#endif
