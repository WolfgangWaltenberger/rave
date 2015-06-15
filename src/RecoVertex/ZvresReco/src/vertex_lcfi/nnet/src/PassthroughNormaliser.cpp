#include "PassthroughNormaliser.h"
#include "NeuralNet.h"

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

PassthroughNormaliser::PassthroughNormaliser(const NeuralNet *parentNetwork)
: InputNormaliser(parentNetwork)
{
}

PassthroughNormaliser::~PassthroughNormaliser(void)
{
}

double PassthroughNormaliser::normalisedValue(const double input) const
{
    return input;
}

void PassthroughNormaliser::serialise(std::ostream &os) const
{
    if (_parentNetwork != (NeuralNet *)0)
    {
        NeuralNet::SerialisationMode mode = _parentNetwork->getSerialisationMode();
        if (mode == NeuralNet::XML)
            os << "<InputNormaliser Type=\"" << name() << "\"/>";
        else if (mode == NeuralNet::PlainText)
            os << name();
    }
}

InputNormaliser *PassthroughNormaliser::clone(const NeuralNet *newNetwork) const
{
    return new PassthroughNormaliser(newNetwork);
}
