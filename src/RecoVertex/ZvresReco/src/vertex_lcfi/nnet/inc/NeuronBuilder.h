#ifndef NEURONBUILDER_H
#define NEURONBUILDER_H

#include "NeuralNetConfig.h"
#include <string>
#include <vector>

#ifdef __CINT__
#include "Neuron.h"
#include "NeuralNet.h"
#else
//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{
class Neuron;
class NeuralNet;
}
#endif

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
NeuronBuilder
{
public:
    NeuronBuilder(void) : _theNetwork(0) {}
	virtual ~NeuronBuilder(void) {}
	virtual Neuron *buildNeuron(const int NumberOfInputs,const double bias=-1.0) const = 0;
	virtual Neuron *buildNeuron(const int numberOfInputs,const std::vector<double> &constructionData) const = 0;
	virtual std::string buildsType() const = 0;
    void setNetwork(const NeuralNet *theNetwork) {_theNetwork = theNetwork;}

protected:
    const NeuralNet *_theNetwork;
};

}//namespace nnet

#endif
