#include "RaveBase/RaveInterface/rave/Covariance6D.h"

namespace rave
{

Covariance6D::Covariance6D ( float dxx, float dxy, float dxz,
                             float dyy, float dyz, float dzz,
                             float dxpx, float dxpy, float dxpz,
                             float dypx, float dypy, float dypz,
                             float dzpx, float dzpy, float dzpz,
                             float dpxpx, float dpxpy, float dpxpz,
                             float dpypy, float dpypz, float dpzpz ) :
    thedxx ( dxx ), thedxy ( dxy ), thedxz ( dxz ), thedyy ( dyy ), thedyz ( dyz ), thedzz ( dzz ),
    thedxpx ( dxpx ), thedxpy ( dxpy ), thedxpz ( dxpz ), thedypx ( dypx ), thedypy ( dypy ), thedypz ( dypz ),
    thedzpx ( dzpx ), thedzpy ( dzpy ), thedzpz ( dzpz ),
    thedpxpx ( dpxpx ), thedpxpy ( dpxpy ), thedpxpz ( dpxpz ),
    thedpypy ( dpypy ), thedpypz ( dpypz ), thedpzpz ( dpzpz )
{}

Covariance6D::Covariance6D() :
    thedxx ( 0. ), thedxy ( 0. ), thedxz ( 0. ), thedyy ( 0. ), thedyz ( 0. ), thedzz ( 0. ),
    thedxpx ( 0. ), thedxpy ( 0. ), thedxpz ( 0. ), thedypx ( 0. ), thedypy ( 0. ), thedypz ( 0. ),
    thedzpx ( 0. ), thedzpy ( 0. ), thedzpz ( 0. ),
    thedpxpx ( 0. ), thedpxpy ( 0. ), thedpxpz ( 0. ),
    thedpypy ( 0. ), thedpypz ( 0. ), thedpzpz ( 0. )
{}


float Covariance6D::dxx() const
{
  return thedxx;
}

float Covariance6D::dxy() const
{
  return thedxy;
}

float Covariance6D::dxz() const
{
  return thedxz;
}

float Covariance6D::dyy() const
{
  return thedyy;
}

float Covariance6D::dyz() const
{
  return thedyz;
}

float Covariance6D::dzz() const
{
  return thedzz;
}

float Covariance6D::dpxpx() const
{
  return thedpxpx;
}

float Covariance6D::dpxpy() const
{
  return thedpxpy;
}

float Covariance6D::dpxpz() const
{
  return thedpxpz;
}

float Covariance6D::dpypy() const
{
  return thedpypy;
}

float Covariance6D::dpypz() const
{
  return thedpypz;
}

float Covariance6D::dpzpz() const
{
  return thedpzpz;
}

float Covariance6D::dxpx() const
{
  return thedxpx;
}

float Covariance6D::dxpy() const
{
  return thedxpy;
}

float Covariance6D::dxpz() const
{
  return thedxpz;
}

float Covariance6D::dypx() const
{
  return thedypx;
}

float Covariance6D::dypy() const
{
  return thedypy;
}

float Covariance6D::dypz() const
{
  return thedypz;
}

float Covariance6D::dzpx() const
{
  return thedzpx;
}

float Covariance6D::dzpy() const
{
  return thedzpy;
}

float Covariance6D::dzpz() const
{
  return thedzpz;
}

bool Covariance6D::equal ( const Covariance6D & other ) const
{
  return ( dxx() == other.dxx() ) &&
         ( dxy() == other.dxy() ) &&
         ( dxz() == other.dxz() ) &&
         ( dyy() == other.dyy() ) &&
         ( dyz() == other.dyz() ) &&
         ( dzz() == other.dzz() ) &&

         ( dxpx() == other.dxpx() ) &&
         ( dxpy() == other.dxpy() ) &&
         ( dxpz() == other.dxpz() ) &&
         ( dypx() == other.dypx() ) &&
         ( dypy() == other.dypy() ) &&
         ( dypz() == other.dypz() ) &&
         ( dzpx() == other.dzpx() ) &&
         ( dzpy() == other.dzpy() ) &&
         ( dzpz() == other.dzpz() ) &&

         ( dpxpx() == other.dpxpx() ) &&
         ( dpxpy() == other.dpxpy() ) &&
         ( dpxpz() == other.dpxpz() ) &&
         ( dpypy() == other.dpypy() ) &&
         ( dpypz() == other.dpypz() ) &&
         ( dpzpz() == other.dpzpz() );
}

bool operator== ( const rave::Covariance6D & lhs, const rave::Covariance6D & rhs )
{
  return lhs.equal ( rhs );
}

} // namespace rave
