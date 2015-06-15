#ifndef NEURONBUILDERCATALOGUE_H
#define NEURONBUILDERCATALOGUE_H

#include "NeuralNetConfig.h"

#include <vector>
#include <string>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class NeuronBuilder;

class NEURALNETDLL NeuronBuilderCatalogue
{
public:
	~NeuronBuilderCatalogue(void);
	static NeuronBuilderCatalogue *instance();
	NeuronBuilder *builderOf(const std::string &neuronType) const;
	NeuronBuilder *builderOf(const char *neuronType) const;
	void destroy() const;
	void addBuilder(NeuronBuilder *theBuilder);

protected:
	NeuronBuilderCatalogue();

private:
	static NeuronBuilderCatalogue *_instance;
	std::vector<NeuronBuilder *> _theBuilders;
};

}//namespace nnet

#endif
