#include "RaveBase/RaveInterface/rave/Ellipsoid3D.h"

using namespace rave;

Ellipsoid3D::Ellipsoid3D ( const Point3D & p, const Covariance3D & c ) :
  theValid(true), thePoint ( p ), theCov ( c )
{}

Ellipsoid3D::Ellipsoid3D () : theValid(false)
{}

bool Ellipsoid3D::isValid() const
{
  return theValid;
}

const Point3D & Ellipsoid3D::point() const
{
  return thePoint;
}

const Covariance3D & Ellipsoid3D::covariance() const
{
  return theCov;
}
