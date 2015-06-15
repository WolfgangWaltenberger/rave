#ifndef _RaveEllipsoid3D_H_
#define _RaveEllipsoid3D_H_

#include <cmath>
#include <rave/Point3D.h>
#include <rave/Covariance3D.h>

namespace rave
{

/** \class RaveEllipsoid3D is a 3d point plus covariance matrix.
 */

class RaveDllExport Ellipsoid3D
{
  public:

    Ellipsoid3D ( const Point3D &, const Covariance3D & );
    Ellipsoid3D ();

    const Point3D & point() const;
    const Covariance3D & covariance() const;
    bool isValid() const;

  private:
    bool theValid;
    Point3D thePoint;
    Covariance3D theCov;
};

}

#endif

