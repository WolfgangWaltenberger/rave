#ifndef RANGEMAPPINGNORMALISERBUILDER_H
#define RANGEMAPPINGNORMALISERBUILDER_H

#include "NeuralNetConfig.h"
#include "InputNormaliserBuilder.h"

#include <vector>
#include <string>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class InputNormaliser;
class NeuralNet;

class NEURALNETDLL RangeMappingNormaliserBuilder :
    public InputNormaliserBuilder
{
public:
    RangeMappingNormaliserBuilder(const NeuralNet *parentNetwork);
    ~RangeMappingNormaliserBuilder(void);

    std::string buildsType() const {return "RangeMappingNormaliser";}
    InputNormaliser *buildNormaliser(const std::vector<double> &constructionData) const;
};

}//namespace nnet

#endif
