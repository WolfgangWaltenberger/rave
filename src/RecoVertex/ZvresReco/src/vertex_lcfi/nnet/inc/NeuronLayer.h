#ifndef NEURONLAYER_H
#define NEURONLAYER_H

#include "NeuralNetConfig.h"

#include <vector>
#include <string>
#include <iostream>

#ifdef __CINT__
#include "Neuron.h"
#include "NeuronBuilder.h"
#include "NeuralNet.h"
#else
//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{
class Neuron;
class NeuronBuilder;
class NeuralNet;
}
#endif

// NeuronLayer is a simple container for Neurons.
// It can only be created on the heap, i.e. you have to new it.

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
NeuronLayer
{
public:
	NeuronLayer(const NeuralNet *parentNetwork=0);
	NeuronLayer(const int numberOfNeurons,const int numberOfInputsPerNeuron,const NeuronBuilder &theNeuronBuilder,const NeuralNet *parentNetwork=0);
	NeuronLayer(const int numberOfInputsPerNeuron,const std::vector<std::string> &namedNeurons,const NeuralNet *parentNetwork=0);
	NeuronLayer(const NeuronLayer &other,const NeuralNet *newParent);
	void destroy() const;
	void serialise(std::ostream &os) const;
	int numberOfNeurons() const {return (int)_theNeurons.size();}
	std::vector<double> output(const std::vector<double> &inputValues) const;
	Neuron *neuron(const int i);
	int numberOfWeights() const;
	void setWeights(const std::vector<double> &newWeights);
	std::vector<double> weights() const;
	std::vector<double> derivativeOutput(const std::vector<double> &inputValues) const;
	void addNeuron(Neuron *neuronToAdd);

protected:
	~NeuronLayer(void);
	void clear();

private:
	std::vector<Neuron *> _theNeurons;
    const NeuralNet *_parentNetwork;

    NeuronLayer(const NeuronLayer &other); // Declared but not defined

};

}//namespace nnet

#endif
