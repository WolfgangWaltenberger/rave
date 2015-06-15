#include "NeuronBuilderCatalogue.h"
#include "SigmoidNeuronBuilder.h"
#include "TanSigmoidNeuronBuilder.h"
#include "LinearNeuronBuilder.h"

#include <iostream>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

NeuronBuilderCatalogue *NeuronBuilderCatalogue::_instance = (NeuronBuilderCatalogue *)0;

NeuronBuilderCatalogue::NeuronBuilderCatalogue()
{
	_theBuilders.clear();
	_theBuilders.push_back(new SigmoidNeuronBuilder);
	_theBuilders.push_back(new TanSigmoidNeuronBuilder);
	_theBuilders.push_back(new LinearNeuronBuilder);
}

NeuronBuilderCatalogue::~NeuronBuilderCatalogue()
{
	if ((int)_theBuilders.size() != 0)
	{
		for (std::vector<NeuronBuilder *>::iterator iter = _theBuilders.begin();
			iter != _theBuilders.end();++iter)
			if ((*iter) != (NeuronBuilder *)0)
				delete (*iter);
		_theBuilders.clear();
	}
}

NeuronBuilderCatalogue *NeuronBuilderCatalogue::instance()
{
	if (_instance == (NeuronBuilderCatalogue *)0)
		_instance = new NeuronBuilderCatalogue();
	return _instance;
}

void NeuronBuilderCatalogue::destroy() const
{
	_instance = (NeuronBuilderCatalogue *)0;
    delete this;
}

NeuronBuilder *NeuronBuilderCatalogue::builderOf(const std::string &neuronType) const
{
	for (std::vector<NeuronBuilder *>::const_iterator iter = _theBuilders.begin();
			iter != _theBuilders.end();++iter)
	{
		if ( (*iter)->buildsType().compare(neuronType)==0 )
			return (*iter);
	}
	return (NeuronBuilder *)0;
}

NeuronBuilder *NeuronBuilderCatalogue::builderOf(const char *neuronType) const
{
	std::string type = neuronType;
	return builderOf(type);
}

void NeuronBuilderCatalogue::addBuilder(NeuronBuilder *theBuilder)
{
	for (std::vector<NeuronBuilder *>::iterator iter = _theBuilders.begin();
			iter != _theBuilders.end();++iter)
	{
		if ((*iter)->buildsType().compare(theBuilder->buildsType()) == 0)
		{
			delete (*iter);
			(*iter) = theBuilder;
			std::cout << "NeuronBuilderCatalogue:: warning - existing builder replaced for ";
			std::cout << theBuilder->buildsType() << std::endl;
			return;
		}
	}
	_theBuilders.push_back(theBuilder);
}
