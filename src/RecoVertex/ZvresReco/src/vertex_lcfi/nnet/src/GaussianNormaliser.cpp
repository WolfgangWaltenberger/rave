#include "GaussianNormaliser.h"
#include "NeuralNet.h"

#include <cmath>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

GaussianNormaliser::GaussianNormaliser(const double mean,const double variance,const NeuralNet *parentNetwork)
: InputNormaliser(parentNetwork),_mean(mean),_variance(variance)
{
}

double GaussianNormaliser::normalisedValue(const double input) const
{
    if (_variance > 0.0)
        return (input-_mean)/sqrt(_variance);
    else
        return input;
}

void GaussianNormaliser::serialise(std::ostream &os) const
{
    if (_parentNetwork != (NeuralNet *)0)
    {
        NeuralNet::SerialisationMode mode = _parentNetwork->getSerialisationMode();
        if (mode == NeuralNet::XML)
        {
            os << "<InputNormaliser Type=\"" << name() << "\">" << std::endl;
            os << "<parameter Value=\"" << _mean << "\"/>" << std::endl;
            os << "<parameter Value=\"" << _variance << "\"/>" << std::endl;
            os << "</InputNormaliser>";
        }
        else if (mode == NeuralNet::PlainText)
            os << name() << " " << _mean << " " << _variance;
    }
}

InputNormaliser *GaussianNormaliser::clone(const NeuralNet *newNetwork) const
{
    return new GaussianNormaliser(_mean,_variance,newNetwork);
}
