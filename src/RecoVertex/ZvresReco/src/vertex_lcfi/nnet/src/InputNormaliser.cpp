#include "InputNormaliser.h"

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

NEURALNETDLL
std::ostream &nnet::operator <<(std::ostream &os,const InputNormaliser &in)
{
    in.serialise(os);
    return(os);
}
