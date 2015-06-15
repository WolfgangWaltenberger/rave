#ifndef PASSTHROUGHNORMALISER_H
#define PASSTHROUGHNORMALISER_H

#include "NeuralNetConfig.h"

#include "InputNormaliser.h"

#include <iostream>
#include <string>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class NeuralNet;

class NEURALNETDLL PassthroughNormaliser :
    public InputNormaliser
{
public:
    PassthroughNormaliser(const NeuralNet *parentNetwork);
    ~PassthroughNormaliser(void);

    double normalisedValue(const double input) const;
    std::string name() const {return "PassthroughNormaliser";}
    void serialise(std::ostream &os) const;
    InputNormaliser *clone(const NeuralNet *newNetwork) const;
};

}//namespace nnet

#endif
