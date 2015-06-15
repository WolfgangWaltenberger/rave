#ifndef _RaveCovariance6D_H_
#define _RaveCovariance6D_H_

#include "rave/CheckedFloat.h"
#include "boost/operators.hpp"

namespace rave
{

/**
 * @class RaveCovariance6D a covariance matrix class for the track error
 */

class RaveDllExport Covariance6D :
      boost::equality_comparable<Covariance6D>
{
  public:

    /**
     *  create a track error object from euclidean errors
     */
    Covariance6D ( float dxx, float dxy, float dxz,
                   float dyy, float dyz, float dzz,
                   float dxpx, float dxpy, float dxpz,
                   float dypx, float dypy, float dypz,
                   float dzpx, float dzpy, float dzpz,
                   float dpxpx, float dpxpy, float dpxpz,
                   float dpypy, float dpypz, float dpzpz );

    /**
     *  empty error matrix
     */
    Covariance6D();

    virtual ~Covariance6D() {};

    float dxx() const;
    float dxy() const;
    float dxz() const;
    float dyy() const;
    float dyz() const;
    float dzz() const;

    float dxpx() const;
    float dxpy() const;
    float dxpz() const;
    float dypx() const;
    float dypy() const;
    float dypz() const;
    float dzpx() const;
    float dzpy() const;
    float dzpz() const;

    float dpxpx() const;
    float dpxpy() const;
    float dpxpz() const;
    float dpypy() const;
    float dpypz() const;
    float dpzpz() const;

    virtual bool equal ( const Covariance6D & other ) const;

  private:
    CheckedFloat thedxx;
    CheckedFloat thedxy;
    CheckedFloat thedxz;
    CheckedFloat thedyy;
    CheckedFloat thedyz;
    CheckedFloat thedzz;

    CheckedFloat thedxpx;
    CheckedFloat thedxpy;
    CheckedFloat thedxpz;
    CheckedFloat thedypx;
    CheckedFloat thedypy;
    CheckedFloat thedypz;
    CheckedFloat thedzpx;
    CheckedFloat thedzpy;
    CheckedFloat thedzpz;

    CheckedFloat thedpxpx;
    CheckedFloat thedpxpy;
    CheckedFloat thedpxpz;
    CheckedFloat thedpypy;
    CheckedFloat thedpypz;
    CheckedFloat thedpzpz;
};

RaveDllExport bool operator== ( const rave::Covariance6D & lhs, const rave::Covariance6D & rhs );

} // namespace rave

#endif
