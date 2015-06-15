#ifndef INPUTNORMALISERBUILDERCATALOGUE_H
#define INPUTNORMALISERBUILDERCATALOGUE_H

#include "NeuralNetConfig.h"

#include <vector>
#include <string>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class InputNormaliserBuilder;
class NeuralNet;

class NEURALNETDLL InputNormaliserBuilderCatalogue
{
public:
    ~InputNormaliserBuilderCatalogue(void);
    static InputNormaliserBuilderCatalogue *instance(const NeuralNet *parentNetwork);
    InputNormaliserBuilder *builderOf(const std::string &normaliserType) const;
    InputNormaliserBuilder *builderOf(const char *normaliserType) const;
    void destroy() const;
    void addNormaliser(InputNormaliserBuilder *theBuilder);

protected:
    const NeuralNet *network() {return _parentNetwork;}
    InputNormaliserBuilderCatalogue(const NeuralNet *parentNetwork);

private:
    static InputNormaliserBuilderCatalogue *_instance;
    const NeuralNet *_parentNetwork;
    std::vector<InputNormaliserBuilder *> _theBuilders;
};

}//namespace nnet

#endif
