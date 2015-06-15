#ifndef SIGMOIDNEURONBUILDER_H
#define SIGMOIDNEURONBUILDER_H

#include "NeuralNetConfig.h"
#include "NeuronBuilder.h"

#include <string>
#include <vector>

#ifdef __CINT__
#include "Neuron.h"
#else
//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{
class Neuron;
}
#endif

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
SigmoidNeuronBuilder
	: public NeuronBuilder
{
public:
	SigmoidNeuronBuilder(void);
	~SigmoidNeuronBuilder(void);
	Neuron *buildNeuron(const int numberOfInputs,const double bias=-1.0) const;
	Neuron *buildNeuron(const int numberOfInputs,const std::vector<double> &constructionData) const;
	void setConstructedNeuronResponse(const double response)
		{ _response = response;}
	std::string buildsType() const
		{ return "SigmoidNeuron";}

private:
	double _response;
};

}//namespace nnet

#endif
