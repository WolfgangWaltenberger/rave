#ifndef Geom_Phi_H
#define Geom_Phi_H

#include "DataFormats/GeometryVector/interface/Pi.h"
#include <cmath>

namespace Geom {

/** A class for azimuthal angle represantation and algebra.
 *  The use of Phi<T> is tranparant due to the implicit conversion to T
 *  Constructs like cos(phi) work as with float or double.
 *  The difference with respect to built-in types is that
 *  Phi is kept in the range [-pi, pi], and this is consistently
 *  implemented in aritmetic operations. In other words, Phi implements 
 *  "modulo(2 pi)" arithmetics.
 */

  template <class T>
  class Phi {
  public:

    /// Default constructor does not initialise - just as double.
    Phi() {}

    /** Constructor from T, does not provide automatic conversion.
     *  The constructor provides range checking and normalization,
     *  e.g. the value of Pi(2*pi()+1) is 1
     */
    explicit Phi( const T& val) : theValue(val) { normalize();}

    /// conversion operator makes transparent use possible.
    operator T() const { return theValue;}

    /// Explicit access to value in case implicit conversion not OK
    T value() const { return theValue;}

    /// Standard arithmetics 
    Phi& operator+=(const T& a) {theValue+=a; normalize(); return *this;}
    Phi& operator+=(const Phi& a) {return operator+=(a.value());}

    Phi& operator-=(const T& a) {theValue-=a; normalize(); return *this;}
    Phi& operator-=(const Phi& a) {return operator-=(a.value());}

    Phi& operator*=(const T& a) {theValue*=a; normalize(); return *this;}

    Phi& operator/=(const T& a) {theValue/=a; normalize(); return *this;}

    T degrees() const { return theValue*180./pi();}

  private:

    T theValue;

    void normalize() { 
      if( theValue > twoPi() || theValue < -twoPi()) {
	theValue = fmod( theValue, (T) twoPi());
      }
      if (theValue < -pi()) theValue += twoPi();
      if (theValue >  pi()) theValue -= twoPi();
    }

  };

  template <class T>
  inline Phi<T> operator-(const Phi<T>& a) {return Phi<T>(-a.value());}

  template <class T>
  inline Phi<T> operator+(const Phi<T>& a, const Phi<T>& b) {
    return Phi<T>(a) += b;
  }
  template <class T>
  inline Phi<T> operator+(const Phi<T>& a, const T& b) {
    return Phi<T>(a) += b;
  }
  template <class T>
  inline Phi<T> operator+(const T& a, const Phi<T>& b) {
    return Phi<T>(b) += a;
  }

  template <class T>
  inline Phi<T> operator-(const Phi<T>& a, const Phi<T>& b) { 
    return Phi<T>(a) -= b;
  }
  template <class T>
  inline Phi<T> operator-(const Phi<T>& a, const T& b) { 
    return Phi<T>(a) -= b;
  }
  template <class T>
  inline Phi<T> operator-(const T& a, const Phi<T>& b) { 
    return Phi<T>(a - b.value());  // use of unary operators normalizes twice
  }

  template <class T>
  inline Phi<T> operator*(const Phi<T>& a, const T& b) {
    return Phi<T>(a) *= b;
  }
  template <class T>
  inline Phi<T> operator*(double a, const Phi<T>& b) {
    return Phi<T>(b) *= a;
  }

  template <class T>
  inline T operator/(const Phi<T>& a, const Phi<T>& b) { 
    return a.value() / b.value();
  }
  template <class T>
  inline Phi<T> operator/(const Phi<T>& a, double b) {
    return Phi<T>(a) /= b;
  }


}
#endif











