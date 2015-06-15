#include "PassthroughNormaliserBuilder.h"
#include "PassthroughNormaliser.h"

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

PassthroughNormaliserBuilder::PassthroughNormaliserBuilder(const NeuralNet *parentNetwork)
: InputNormaliserBuilder(parentNetwork)
{
}

PassthroughNormaliserBuilder::~PassthroughNormaliserBuilder(void)
{
}

InputNormaliser *PassthroughNormaliserBuilder::buildNormaliser(const std::vector<double> &constructionData) const
{
    return new PassthroughNormaliser(_parentNetwork);
}
