#include "InputNormaliserBuilderCatalogue.h"
#include "PassthroughNormaliserBuilder.h"
#include "GaussianNormaliserBuilder.h"
#include "RangeMappingNormaliserBuilder.h"

#include <iostream>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

InputNormaliserBuilderCatalogue *InputNormaliserBuilderCatalogue::_instance = (InputNormaliserBuilderCatalogue *)0;

InputNormaliserBuilderCatalogue::InputNormaliserBuilderCatalogue(const NeuralNet *parentNetwork)
: _parentNetwork(parentNetwork)
{
    _theBuilders.clear();
    _theBuilders.push_back(new PassthroughNormaliserBuilder(_parentNetwork));
    _theBuilders.push_back(new GaussianNormaliserBuilder(_parentNetwork));
    _theBuilders.push_back(new RangeMappingNormaliserBuilder(_parentNetwork));
}

InputNormaliserBuilderCatalogue::~InputNormaliserBuilderCatalogue(void)
{
    if ((int)_theBuilders.size() != 0)
    {
 		for (std::vector<InputNormaliserBuilder *>::iterator iter = _theBuilders.begin();
			iter != _theBuilders.end();++iter)
			if ((*iter) != (InputNormaliserBuilder *)0)
				delete (*iter);
		_theBuilders.clear();
	}
}

InputNormaliserBuilderCatalogue *InputNormaliserBuilderCatalogue::instance(const NeuralNet *parentNetwork)
{
    if (_instance == (InputNormaliserBuilderCatalogue *)0)
        _instance = new InputNormaliserBuilderCatalogue(parentNetwork);
    else if (_instance->network() != parentNetwork)
    {
        delete _instance;
        _instance = new InputNormaliserBuilderCatalogue(parentNetwork);
    }
    return _instance;
}

InputNormaliserBuilder *InputNormaliserBuilderCatalogue::builderOf(const std::string &normaliserType) const
{
	for (std::vector<InputNormaliserBuilder *>::const_iterator iter = _theBuilders.begin();
			iter != _theBuilders.end();++iter)
	{
		if ( (*iter)->buildsType().compare(normaliserType)==0 )
			return (*iter);
	}
	return (InputNormaliserBuilder *)0;
}

InputNormaliserBuilder *InputNormaliserBuilderCatalogue::builderOf(const char *normaliserType) const
{
    std::string type = normaliserType;
    return builderOf(type);
}

void InputNormaliserBuilderCatalogue::destroy() const
{
    _instance = (InputNormaliserBuilderCatalogue *)0;
    delete this;
}

void InputNormaliserBuilderCatalogue::addNormaliser(InputNormaliserBuilder *theBuilder)
{
	for (std::vector<InputNormaliserBuilder *>::iterator iter = _theBuilders.begin();
			iter != _theBuilders.end();++iter)
	{
		if ((*iter)->buildsType().compare(theBuilder->buildsType()) == 0)
		{
			delete (*iter);
			(*iter) = theBuilder;
			std::cout << "InputNormaliserBuilderCatalogue:: warning - existing builder replaced for ";
			std::cout << theBuilder->buildsType() << std::endl;
			return;
		}
	}
	_theBuilders.push_back(theBuilder);
}
