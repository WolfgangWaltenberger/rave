#include "rave/CheckedFloat.h"

#include <cmath>

rave::CheckedFloat & rave::CheckedFloat::operator=( 
    const InternalType & rhs )
{
  checkedAssign( rhs );
  return *this;
}

void rave::CheckedFloat::checkedAssign( 
    const InternalType & value )
{ 
  // Perform the checks here and throw if necessary
  if ( std::isnan( value ) ) {
    throw CheckedFloatException();
  }
  theValue = value;
}
   
