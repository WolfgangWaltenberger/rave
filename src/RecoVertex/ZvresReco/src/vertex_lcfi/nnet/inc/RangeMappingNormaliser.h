#ifndef RANGEMAPPINGNORMALISER_H
#define RANGEMAPPINGNORMALISER_H

#include "NeuralNetConfig.h"
#include "InputNormaliser.h"

#include <iostream>
#include <string>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class NEURALNETDLL RangeMappingNormaliser :
    public InputNormaliser
{
public:
    RangeMappingNormaliser(const double inputRangeMin,const double inputRangeMax,
                                const double outputRangeMin,const double outputRangeMax,
                                const NeuralNet *parentNetwork);
    ~RangeMappingNormaliser(void);

    double normalisedValue(const double input) const;
    std::string name() const {return "RangeMappingNormaliser";}
    void serialise(std::ostream &os) const;
    InputNormaliser *clone(const NeuralNet *newNetwork) const;

private:
    double _inputMin;
    double _inputRange;
    double _outputMin;
    double _outputRange;
};

}//namespace nnet

#endif
