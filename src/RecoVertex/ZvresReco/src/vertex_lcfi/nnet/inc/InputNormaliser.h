#ifndef INPUTNORMALISER_H
#define INPUTNORMALISER_H

#include "NeuralNetConfig.h"

#ifdef __CINT__
#include "NeuralNet.h"
#else
//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{
class NeuralNet;
}
#endif

#include <iostream>
#include <string>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
InputNormaliser
{
public:
    InputNormaliser(const NeuralNet *parentNetwork) : _parentNetwork(parentNetwork) {}
    virtual ~InputNormaliser(){}

    virtual double normalisedValue(const double input) const = 0;
    virtual std::string name() const = 0;
    virtual void serialise(std::ostream &os) const = 0;
    virtual InputNormaliser *clone(const NeuralNet *newNetwork) const = 0;

protected:
    const NeuralNet *_parentNetwork;

private:
    InputNormaliser(const InputNormaliser &other); // Declared but not defined
};

#ifndef __CINT__
NEURALNETDLL
#endif
std::ostream &operator <<(std::ostream &os,const InputNormaliser &in);

}//namespace nnet

#endif
