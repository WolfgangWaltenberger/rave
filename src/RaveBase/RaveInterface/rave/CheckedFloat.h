#ifndef _CheckedFloat_H_
#define _CheckedFloat_H_

#include <exception>

namespace rave {

class RaveDllExport CheckedFloatException : 
  public std::exception
{
  public:
    virtual const char* what() const throw()
    {
      return "rave/CheckedFloat.h: The assigned floating point number cannot be used.";
    }
};

/**
 * @class CheckedFloat wraps the float type into various checking routines
 */

class RaveDllExport CheckedFloat
{
  typedef float InternalType;
  public:
    CheckedFloat( InternalType value ) { checkedAssign( value ); };
    CheckedFloat & operator=( const InternalType & rhs );
    operator InternalType() const { return theValue; };
  protected:
    InternalType theValue;
    void checkedAssign( const InternalType & value );
};

}

#endif

