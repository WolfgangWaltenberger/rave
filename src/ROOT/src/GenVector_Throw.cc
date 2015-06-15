#include "Math/GenVector/GenVector_exception.h"
#include "Math/GenVector/BitReproducible.h"

namespace ROOT {
namespace Math {
namespace GenVector {

void Throw ( char const * e )
{
  throw e;
}
}

/*
namespace GenVector_detail {
   void BitReproducible::Dto2longs(double d, unsigned int & i1, unsigned int & i2 ) {}
}*/

}
}
