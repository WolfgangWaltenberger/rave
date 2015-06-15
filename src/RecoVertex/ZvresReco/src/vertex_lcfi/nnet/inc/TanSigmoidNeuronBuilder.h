#ifndef TANSIGMOIDNEURONBUILDER_H
#define TANSIGMOIDNEURONBUILDER_H

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
TanSigmoidNeuronBuilder
	: public NeuronBuilder
{
public:
	TanSigmoidNeuronBuilder(void);
	~TanSigmoidNeuronBuilder(void);
	Neuron *buildNeuron(const int numberOfInputs,const double bias=-1.0) const;
	Neuron *buildNeuron(const int numberOfInputs,const std::vector<double> &constructionData) const;
	void setConstructedNeuronScale(const double scale)
		{ _scale = scale;}
	std::string buildsType() const
		{ return "TanSigmoidNeuron";}

private:
	double _scale;
};

}//namespace nnet

#endif
