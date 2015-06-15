#ifndef _RaveCovariance3To6D_H_
#define _RaveCovariance3To6D_H_

#include "rave/CheckedFloat.h"
#include "boost/operators.hpp"

namespace rave
{

/**
 * @class RaveCovariance3To6D is for the track-to-vertex-covariance
 */

class RaveDllExport Covariance3To6D :
      boost::equality_comparable<Covariance3To6D>
{
  public:

    /**
     *  constructor of the track-to-vertex covariance  
     */
    Covariance3To6D ( float dxx, float dxy, float dxz,
                   float dyy, float dyz, float dzz,
                   float dxpx, float dxpy, float dxpz,
                   float dypx, float dypy, float dypz,
                   float dzpx, float dzpy, float dzpz );

    /**
     *  empty error matrix
     */
    Covariance3To6D();

    virtual ~Covariance3To6D() {};

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

    virtual bool equal ( const Covariance3To6D & other ) const;

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
};

RaveDllExport bool operator== ( const rave::Covariance3To6D & lhs, const rave::Covariance3To6D & rhs );

} // namespace rave

#endif
