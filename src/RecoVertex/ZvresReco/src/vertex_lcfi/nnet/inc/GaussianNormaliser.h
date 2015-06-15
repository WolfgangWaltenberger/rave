#ifndef GAUSSIANNORMALISER_H
#define GAUSSIANNORMALISER_H

#include "NeuralNetConfig.h"

#include "InputNormaliser.h"

#include <iostream>
#include <string>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class NeuralNet;

class NEURALNETDLL GaussianNormaliser :
    public InputNormaliser
{
public:
    GaussianNormaliser(const double mean,const double variance,const NeuralNet *parentNetwork);
    ~GaussianNormaliser(void){}

    double normalisedValue(const double input) const;
    std::string name() const {return "GaussianNormaliser";}
    void serialise(std::ostream &os) const;
    InputNormaliser *clone(const NeuralNet *newNetwork) const;

private:
    double _mean;
    double _variance;
};

}//namespace nnet

#endif
