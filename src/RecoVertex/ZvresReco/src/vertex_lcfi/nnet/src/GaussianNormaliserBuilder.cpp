#include "GaussianNormaliserBuilder.h"
#include "GaussianNormaliser.h"

#include <iostream>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

GaussianNormaliserBuilder::GaussianNormaliserBuilder(const NeuralNet *parentNetwork)
: InputNormaliserBuilder(parentNetwork)
{
}

GaussianNormaliserBuilder::~GaussianNormaliserBuilder(void)
{
}

InputNormaliser *GaussianNormaliserBuilder::buildNormaliser(const std::vector<double> &constructionData) const
{
    if ((int)constructionData.size()<2)
    {
        std::cerr << "GaussianNormaliserBuilder:: Cannot construct - insufficient construction data" << std::endl;
        return (InputNormaliser *)0;
    }
    else if (constructionData[1] <= 0.0)
    {
        std::cerr << "GaussianNormaliserBuilder:: Cannot construct - negative or zero variance" << std::endl;
        return (InputNormaliser *)0;
    }
    else
    {
        return new GaussianNormaliser(constructionData[0],constructionData[1],_parentNetwork);
    }
}
