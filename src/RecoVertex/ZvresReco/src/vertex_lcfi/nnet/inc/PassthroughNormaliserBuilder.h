#ifndef PASSTHROUGHNORMALISERBUILDER_H
#define PASSTHROUGHNORMALISERBUILDER_H

#include "NeuralNetConfig.h"
#include "InputNormaliserBuilder.h"
#include <string>
#include <vector>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class InputNormaliser;
class NeuralNet;

class NEURALNETDLL PassthroughNormaliserBuilder :
    public InputNormaliserBuilder
{
public:
    PassthroughNormaliserBuilder(const NeuralNet *parentNetwork);
    ~PassthroughNormaliserBuilder(void);

    std::string buildsType() const {return "PassthroughNormaliser";}
    InputNormaliser *buildNormaliser(const std::vector<double> &constructionData) const;
};

}//namespace nnet

#endif
