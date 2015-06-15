#ifndef _Measurement1D_H_
#define _Measurement1D_H_

#include <string>

/*!
 * @class Measurement1D
 * @brief Interface class for a value/error pair
 * 
 * A class that combines a value and it's associated uncertainty,
 * or error, together. Provides a more explicit interface than
 * a pair<double,double>. If you don't like the name, propose a better one!
 * 
 */
class Measurement1D
{
public:
    //! Empty constructor initializes both value and error to zero
    Measurement1D() : theValue(0.) , theError(0.)
    {}

    //! Constructor for a value without an error
    Measurement1D( const double& aValue) :
            theValue(aValue) , theError(0.)
    {}

    //! Default initialization constructor for a given value/error pair
    Measurement1D( const double& aValue, const double& aError)
            : theValue(aValue) , theError(aError)
    {}

    //! Empty destructor
    ~Measurement1D()
    {}

    //! Read-only property getter for the value
    double value() const
    {
        return theValue;
    }

    //! Read-only property getter for the error
    double error() const
    {
        return theError;
    }

    //! Weights the value with the error and returns zero on  missing error
    double significance() const
    {
        if (theError == 0)
            return 0;
        else
            return theValue/theError;
    }

private:
    double  theValue;		//!< stores the measurement value
    double  theError;		//!< stores the measurement error

};

#endif
