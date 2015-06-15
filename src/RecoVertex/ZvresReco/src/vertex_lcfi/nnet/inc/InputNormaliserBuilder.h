#ifndef INPUTNORMALISERBUILDER_H
#define INPUTNORMALISERBUILDER_H

#include "NeuralNetConfig.h"
#include <string>
#include <vector>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class InputNormaliser;
class NeuralNet;

class NEURALNETDLL InputNormaliserBuilder
{
public:
    InputNormaliserBuilder(const NeuralNet *parentNetwork) : _parentNetwork(parentNetwork) {}
    virtual ~InputNormaliserBuilder(void) {}

    virtual std::string buildsType() const = 0;
    virtual InputNormaliser *buildNormaliser(const std::vector<double> &constructionData) const = 0;

protected:
    const NeuralNet *_parentNetwork;
};

}//namespace nnet

#endif
