#include "RaveBase/RaveInterface/rave/Covariance33D.h"

namespace rave
{

Covariance33D::Covariance33D ( float dxpx, float dxpy, float dxpz,
                               float dypx, float dypy, float dypz, 
                               float dzpx, float dzpy, float dzpz ) :
    thedxpx ( dxpx ), thedxpy ( dxpy ), thedxpz ( dxpz ), 
    thedypx ( dypx ), thedypy ( dypy ), thedypz ( dypz ), 
    thedzpx ( dzpx ), thedzpy ( dzpy ), thedzpz ( dzpz )
{}

Covariance33D::Covariance33D() :
    thedxpx ( 0. ), thedxpy ( 0. ), thedxpz ( 0. ), 
    thedypx ( 0. ), thedypy ( 0. ), thedypz ( 0. ), 
    thedzpx ( 0. ), thedzpy ( 0. ), thedzpz ( 0. )
{}

float Covariance33D::dxpx() const
{
  return thedxpx;
}

float Covariance33D::dxpy() const
{
  return thedxpy;
}

float Covariance33D::dxpz() const
{
  return thedxpz;
}

float Covariance33D::dypx() const
{
  return thedypx;
}

float Covariance33D::dypy() const
{
  return thedypy;
}

float Covariance33D::dypz() const
{
  return thedypz;
}

float Covariance33D::dzpx() const
{
  return thedzpx;
}

float Covariance33D::dzpy() const
{
  return thedzpy;
}


float Covariance33D::dzpz() const
{
  return thedzpz;
}

bool Covariance33D::equal ( const Covariance33D & other ) const
{
  return ( dxpx() == other.dxpx() ) &&
         ( dxpy() == other.dxpy() ) &&
         ( dxpz() == other.dxpz() ) &&
         ( dypx() == other.dypx() ) &&
         ( dypy() == other.dypy() ) &&
         ( dypz() == other.dypz() ) &&
         ( dzpx() == other.dzpx() ) &&
         ( dzpy() == other.dzpy() ) &&
         ( dzpz() == other.dzpz() );
}

bool operator== ( const rave::Covariance33D & lhs, const rave::Covariance33D & rhs )
{
  return lhs.equal ( rhs );
}

} // namespace rave
