#include "NeuronLayer.h"
#include "Neuron.h"
#include "NeuronBuilder.h"
#include "NeuronBuilderCatalogue.h"
#include "NeuralNet.h"

#include <exception>
#include <stdexcept>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

NeuronLayer::NeuronLayer(const NeuralNet *parentNetwork)
: _parentNetwork(parentNetwork)
{
	_theNeurons.clear();
}

NeuronLayer::NeuronLayer(const int numberOfNeurons,const int numberOfInputsPerNeuron,const NeuronBuilder &theNeuronBuilder,const NeuralNet *parentNetwork)
: _parentNetwork(parentNetwork)
{
	for (int i=0;i<numberOfNeurons;++i)
		_theNeurons.push_back(theNeuronBuilder.buildNeuron(numberOfInputsPerNeuron));
}

NeuronLayer::NeuronLayer(const int numberOfInputsPerNeuron,const std::vector<std::string> &namedNeurons,const NeuralNet *parentNetwork)
: _parentNetwork(parentNetwork)
{
	for (int i=0;i<(int)namedNeurons.size();++i)
	{
		NeuronBuilder *theBuilder = NeuronBuilderCatalogue::instance()->builderOf(namedNeurons[i]);
		if (theBuilder != (NeuronBuilder *)0)
        {
            theBuilder->setNetwork(_parentNetwork);
			_theNeurons.push_back(theBuilder->buildNeuron(numberOfInputsPerNeuron));
        }
		else
			std::cerr << "NeuronLayer:: Error - no builder in catalogue for neuron type " << namedNeurons[i] << std::endl;
	}
}

NeuronLayer::NeuronLayer(const NeuronLayer &other,const NeuralNet *newParent)
{
    _parentNetwork = newParent;
	NeuronLayer &theother = const_cast<NeuronLayer &>(other);
	clear();
	for (int i=0;i<other.numberOfNeurons();++i)
		_theNeurons.push_back(theother.neuron(i)->clone(newParent));
}

NeuronLayer::~NeuronLayer(void)
{
	clear();
}

void NeuronLayer::destroy() const
{
	delete this;
}

void NeuronLayer::clear()
{
	for (std::vector<Neuron *>::iterator iter=_theNeurons.begin();
		iter != _theNeurons.end();++iter)
		(*iter)->destroy();
	_theNeurons.clear();
}

void NeuronLayer::serialise(std::ostream &os) const
{
    if (_parentNetwork != (NeuralNet *)0)
    {
        NeuralNet::SerialisationMode mode = _parentNetwork->getSerialisationMode();
        if (mode == NeuralNet::XML)
        {
	        os << "<NeuronLayer NumberOfNeurons=\"" << (int)_theNeurons.size() << "\">" << std::endl;
	        for (std::vector<Neuron *>::const_iterator iter = _theNeurons.begin();
		        iter != _theNeurons.end();++iter)
		        (*iter)->serialise(os);
	        os << "</NeuronLayer>" << std::endl;
        }
        else if (mode == NeuralNet::PlainText)
        {
	        os << (int)_theNeurons.size() << std::endl;
	        for (std::vector<Neuron *>::const_iterator iter = _theNeurons.begin();
		        iter != _theNeurons.end();++iter)
		    (*iter)->serialise(os);
        }
    }
}

std::vector<double> NeuronLayer::output(const std::vector<double> &inputValues) const
{
	std::vector<double> theNeuronOutputs;
	for (int i=0;i<numberOfNeurons();++i)
		theNeuronOutputs.push_back(_theNeurons[i]->output(inputValues));
	return theNeuronOutputs;
}

Neuron *NeuronLayer::neuron(const int i)
{
	if (((i>=0)&&(i<(int)_theNeurons.size()))&&!_theNeurons.empty())
		return _theNeurons[i];
	else
		throw std::out_of_range("Attempt to access non existant neuron.");
}

int NeuronLayer::numberOfWeights() const
{
	return _theNeurons[0]->numberOfWeights()*numberOfNeurons();
}

void NeuronLayer::setWeights(const std::vector<double> &newWeights)
{
	if ((int)newWeights.size()<numberOfWeights())
		std::cerr << "NeuronLayer:: Too few weights supplied to set layer weights." << std::endl;
	else
	{
		std::vector<double> newNeuronWeights;
		std::vector<double>::const_iterator iter = newWeights.begin();
		for (int i=0;i<numberOfNeurons();++i)
		{
			int weightsPerNeuron = _theNeurons[i]->numberOfWeights();
			newNeuronWeights.assign(iter,iter+weightsPerNeuron);
			_theNeurons[i]->setWeights(newNeuronWeights);
			iter+=weightsPerNeuron;
		}
	}
}

std::vector<double> NeuronLayer::weights() const
{
	std::vector<double> theWeights;
	for (int i=0;i<numberOfNeurons();++i)
	{
		std::vector<double> neuronWeights = _theNeurons[i]->weights();
		theWeights.insert(theWeights.end(),neuronWeights.begin(),neuronWeights.end());
	}
	return theWeights;
}

std::vector<double> NeuronLayer::derivativeOutput(const std::vector<double> &inputValues) const
{
	std::vector<double> theNeuronOutputs;
	for (int i=0;i<numberOfNeurons();++i)
		theNeuronOutputs.push_back(_theNeurons[i]->derivativeOutput(inputValues));
	return theNeuronOutputs;
}

void NeuronLayer::addNeuron(Neuron *neuronToAdd)
{
	if (neuronToAdd != (Neuron *)0)
		_theNeurons.push_back(neuronToAdd);
}
