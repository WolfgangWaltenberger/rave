#ifndef _RaveCovariance33D_H_
#define _RaveCovariance33D_H_

#include "rave/CheckedFloat.h"

namespace rave
{

/**
 * @class RaveCovariance33D a class for vertex(position) errors.
 */

class RaveDllExport Covariance33D
  //: boost::equality_comparable<Covariance33D>
{
  public:

    /**
     *   Creates the error object for a positional error
     */
    Covariance33D( float dxpx, float dxpy, float dxpz,
                   float dypx, float dypy, float dypz, 
                   float dzpx, float dzpy, float dzpz );
    Covariance33D();

    float dxpx() const;
    float dxpy() const;
    float dxpz() const;
    float dypx() const;
    float dypy() const;
    float dypz() const;
    float dzpx() const;
    float dzpy() const;
    float dzpz() const;

    bool equal(const Covariance33D & other) const;

  private:
    CheckedFloat thedxpx;
    CheckedFloat thedxpy;
    CheckedFloat thedxpz;
    CheckedFloat thedypx;
    CheckedFloat thedypy;
    CheckedFloat thedypz;
    CheckedFloat thedzpx;
    CheckedFloat thedzpy;
    CheckedFloat thedzpz;
};

RaveDllExport bool operator==(const rave::Covariance33D & lhs, const rave::Covariance33D & rhs);

} // namespace rave

#endif
