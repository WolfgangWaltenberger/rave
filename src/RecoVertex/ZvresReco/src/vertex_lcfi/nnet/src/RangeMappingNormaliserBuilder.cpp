#include "RangeMappingNormaliserBuilder.h"
#include "RangeMappingNormaliser.h"

#include <iostream>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

RangeMappingNormaliserBuilder::RangeMappingNormaliserBuilder(const NeuralNet *parentNetwork)
: InputNormaliserBuilder(parentNetwork)
{
}

RangeMappingNormaliserBuilder::~RangeMappingNormaliserBuilder(void)
{
}

InputNormaliser *RangeMappingNormaliserBuilder::buildNormaliser(const std::vector<double> &constructionData) const
{
    if ((int)constructionData.size() < 4)
    {
        std::cerr << "RangeMappingNormaliserBuilder:: error - not enough construction data supplied to build object" << std::endl;
        return (InputNormaliser *)0;
    }
    else
        return new RangeMappingNormaliser(constructionData[0],constructionData[0]+constructionData[1],
                                          constructionData[2],constructionData[2]+constructionData[3],
                                          _parentNetwork);
}
