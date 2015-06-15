#ifndef Geom_Theta_H
#define Geom_Theta_H

#include <DataFormats/GeometryVector/interface/Pi.h>
#include <cmath>

namespace Geom {

/** A class for polar angle represantation.
 *  The use of Theta<T> is tranparant due to the implicit conversion to T
 *  Constructs like cos(theta) work as with float or double.
 *  The difference with respect to built-in types is that
 *  Theta is kept in the range [0, pi), and this is consistently
 *  implemented in aritmetic operations. In other words, Theta implements 
 *  "modulo(pi)" arithmetics.
 */

  template <class T>
  class Theta {
  public:

    /// Default constructor does not initialise - just as double.
    Theta() {}

    /// Constructor from T, does not provide automatic conversion.
    explicit Theta( const T& val) : theValue(val) { normalize(); }

    /// conversion operator makes transparent use possible.
    operator T() const { return theValue;}

    // Template argument conversion
    //    template <class T1> operator Theta<T1>() { return Theta<T1>(theValue);}

    /// Explicit access to value in case implicit conversion not OK
    T value() const { return theValue;}

    // Standard arithmetics 
    Theta& operator+=(const T& a) {theValue+=a; normalize(); return *this;}
    Theta& operator+=(const Theta& a) {return operator+=(a.value());}

    Theta& operator-=(const T& a) {theValue-=a; normalize(); return *this;}
    Theta& operator-=(const Theta& a) {return operator-=(a.value());}

    Theta& operator*=(const T& a) {theValue*=a; normalize(); return *this;}

    Theta& operator/=(const T& a) {theValue/=a; normalize(); return *this;}

    T degrees() const { return theValue*180./pi();}

    /// Return the pseudorapidity.
    // No need to handle 0 or pi; in this case "inf" is returned.
    T eta() const { return -log(tan(theValue/2.)); } 
    

  private:

    T theValue;

    void normalize() {
      if( theValue > pi() || theValue < 0) {
	theValue = fmod( theValue, (T) pi());
      }
      if (theValue <  0.)   theValue += pi();
    }

  };

  template <class T>
  inline Theta<T> operator-(const Theta<T>& a) {return Theta<T>(-a.value());}

  template <class T>
  inline Theta<T> operator+(const Theta<T>& a, const Theta<T>& b) {
    return Theta<T>(a) += b;
  }
  template <class T>
  inline Theta<T> operator+(const Theta<T>& a, const T& b) {
    return Theta<T>(a) += b;
  }
  template <class T>
  inline Theta<T> operator+(const T& a, const Theta<T>& b) {
    return Theta<T>(b) += a;
  }

  template <class T>
  inline Theta<T> operator-(const Theta<T>& a, const Theta<T>& b) { 
    return Theta<T>(a) -= b;
  }
  template <class T>
  inline Theta<T> operator-(const Theta<T>& a, const T& b) { 
    return Theta<T>(a) -= b;
  }
  template <class T>
  inline Theta<T> operator-(const T& a, const Theta<T>& b) { 
    return Theta<T>(a - b.value());  // use of unary operators normalizes twice
  }

  template <class T>
  inline Theta<T> operator*(const Theta<T>& a, const T& b) {
    return Theta<T>(a) *= b;
  }
  template <class T>
  inline Theta<T> operator*(double a, const Theta<T>& b) {
    return Theta<T>(b) *= a;
  }

  template <class T>
  inline T operator/(const Theta<T>& a, const Theta<T>& b) { 
    return a.value() / b.value();
  }
  template <class T>
  inline Theta<T> operator/(const Theta<T>& a, double b) {
    return Theta<T>(a) /= b;
  }

}
#endif
