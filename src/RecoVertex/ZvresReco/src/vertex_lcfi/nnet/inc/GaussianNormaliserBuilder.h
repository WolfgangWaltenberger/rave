#ifndef GAUSSIANNORMALISERBUILDER_H
#define GAUSSIANNORMALISERBUILDER_H

#include "NeuralNetConfig.h"
#include "InputNormaliserBuilder.h"
#include <string>
#include <vector>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class InputNormaliser;
class NeuralNet;

class NEURALNETDLL GaussianNormaliserBuilder :
    public InputNormaliserBuilder
{
public:
    GaussianNormaliserBuilder(const NeuralNet *parentNetwork);
    ~GaussianNormaliserBuilder(void);

    std::string buildsType() const {return "GaussianNormaliser";}
    InputNormaliser *buildNormaliser(const std::vector<double> &constructionData) const;
};

}//namespace nnet

#endif
