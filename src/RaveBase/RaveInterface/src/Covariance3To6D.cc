#include "RaveBase/RaveInterface/rave/Covariance3To6D.h"

namespace rave
{

Covariance3To6D::Covariance3To6D ( float dxx, float dxy, float dxz,
                             float dyy, float dyz, float dzz,
                             float dxpx, float dxpy, float dxpz,
                             float dypx, float dypy, float dypz,
                             float dzpx, float dzpy, float dzpz ) :
    thedxx ( dxx ), thedxy ( dxy ), thedxz ( dxz ), thedyy ( dyy ), thedyz ( dyz ), 
    thedzz ( dzz ), thedxpx ( dxpx ), thedxpy ( dxpy ), thedxpz ( dxpz ), 
    thedypx ( dypx ), thedypy ( dypy ), thedypz ( dypz ),
    thedzpx ( dzpx ), thedzpy ( dzpy ), thedzpz ( dzpz )
{}

Covariance3To6D::Covariance3To6D() :
    thedxx ( 0. ), thedxy ( 0. ), thedxz ( 0. ), thedyy ( 0. ), thedyz ( 0. ), 
    thedzz ( 0. ), thedxpx ( 0. ), thedxpy ( 0. ), thedxpz ( 0. ), thedypx ( 0. ), 
    thedypy ( 0. ), thedypz ( 0. ), thedzpx ( 0. ), thedzpy ( 0. ), thedzpz ( 0. )
{}


float Covariance3To6D::dxx() const
{
  return thedxx;
}

float Covariance3To6D::dxy() const
{
  return thedxy;
}

float Covariance3To6D::dxz() const
{
  return thedxz;
}

float Covariance3To6D::dyy() const
{
  return thedyy;
}

float Covariance3To6D::dyz() const
{
  return thedyz;
}

float Covariance3To6D::dzz() const
{
  return thedzz;
}

float Covariance3To6D::dxpx() const
{
  return thedxpx;
}

float Covariance3To6D::dxpy() const
{
  return thedxpy;
}

float Covariance3To6D::dxpz() const
{
  return thedxpz;
}

float Covariance3To6D::dypx() const
{
  return thedypx;
}

float Covariance3To6D::dypy() const
{
  return thedypy;
}

float Covariance3To6D::dypz() const
{
  return thedypz;
}

float Covariance3To6D::dzpx() const
{
  return thedzpx;
}

float Covariance3To6D::dzpy() const
{
  return thedzpy;
}

float Covariance3To6D::dzpz() const
{
  return thedzpz;
}

bool Covariance3To6D::equal ( const Covariance3To6D & other ) const
{
  return ( ( dxx() == other.dxx() ) &&
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
         ( dzpz() == other.dzpz() ) );
}

bool operator== ( const rave::Covariance3To6D & lhs, 
                  const rave::Covariance3To6D & rhs )
{
  return lhs.equal ( rhs );
}

} // namespace rave
