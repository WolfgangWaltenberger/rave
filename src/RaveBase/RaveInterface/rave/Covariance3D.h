#ifndef _RaveCovariance3D_H_
#define _RaveCovariance3D_H_

#include "rave/CheckedFloat.h"

namespace rave
{

/**
 * @class RaveCovariance3D a class for vertex(position) errors.
 */

class RaveDllExport Covariance3D
  //: boost::equality_comparable<Covariance3D>
{
  public:

    /**
     *   Creates the error object for a positional error
     */
    Covariance3D( float dxx, float dxy, float dxz,
                  float dyy, float dyz, float dzz );
    Covariance3D();

    float dxx() const;
    float dxy() const;
    float dxz() const;
    float dyy() const;
    float dyz() const;
    float dzz() const;

    bool equal(const Covariance3D & other) const;

  private:
    CheckedFloat thedxx;
    CheckedFloat thedxy;
    CheckedFloat thedxz;
    CheckedFloat thedyy;
    CheckedFloat thedyz;
    CheckedFloat thedzz;
};

RaveDllExport bool operator==(const rave::Covariance3D & lhs, const rave::Covariance3D & rhs);

} // namespace rave

#endif
