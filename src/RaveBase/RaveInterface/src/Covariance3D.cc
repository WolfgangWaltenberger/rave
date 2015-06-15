#include "RaveBase/RaveInterface/rave/Covariance3D.h"

namespace rave
{

Covariance3D::Covariance3D ( float dxx, float dxy, float dxz,
                             float dyy, float dyz, float dzz ) :
    thedxx ( dxx ), thedxy ( dxy ), thedxz ( dxz ), thedyy ( dyy ), thedyz ( dyz ), thedzz ( dzz )
{}


Covariance3D::Covariance3D() :
    thedxx ( 0. ), thedxy ( 0. ), thedxz ( 0. ), thedyy ( 0. ), thedyz ( 0. ), thedzz ( 0. )
{}


float Covariance3D::dxx() const
{
  return thedxx;
}

float Covariance3D::dxy() const
{
  return thedxy;
}

float Covariance3D::dxz() const
{
  return thedxz;
}

float Covariance3D::dyy() const
{
  return thedyy;
}

float Covariance3D::dyz() const
{
  return thedyz;
}

float Covariance3D::dzz() const
{
  return thedzz;
}

bool Covariance3D::equal ( const Covariance3D & other ) const
{
  return ( dxx() == other.dxx() ) &&
         ( dxy() == other.dxy() ) &&
         ( dxz() == other.dxz() ) &&
         ( dyy() == other.dyy() ) &&
         ( dyz() == other.dyz() ) &&
         ( dzz() == other.dzz() );
}

bool operator== ( const rave::Covariance3D & lhs, const rave::Covariance3D & rhs )
{
  return lhs.equal ( rhs );
}

} // namespace rave
