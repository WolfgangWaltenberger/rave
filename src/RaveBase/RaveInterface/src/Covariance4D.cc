#include "RaveBase/RaveInterface/rave/Covariance4D.h"

namespace rave
{

Covariance4D::Covariance4D (
  float dxx, float dxy, float dxz, float dxm, float dyy, float dyz,
  float dym, float dzz, float dzm, float dmm ) :
    Covariance3D ( dxx, dxy, dxz, dyy, dyz, dzz ),
    thedxm ( dxm ), thedym ( dym ), thedzm ( dzm ), thedmm ( dmm )
{}

rave::Covariance4D::Covariance4D() :
    Covariance3D (),
    thedxm ( 0. ), thedym ( 0. ), thedzm ( 0. ), thedmm ( 0. )
{}

float rave::Covariance4D::dxm() const
{
  return thedxm;
}

float rave::Covariance4D::dym() const
{
  return thedym;
}

float rave::Covariance4D::dzm() const
{
  return thedzm;
}

float rave::Covariance4D::dmm() const
{
  return thedmm;
}

bool rave::Covariance4D::equal ( const rave::Covariance4D & other ) const
{
  return Covariance3D::equal ( other ) &&
         ( dxm() == other.dxm() ) &&
         ( dym() == other.dym() ) &&
         ( dzm() == other.dzm() ) &&
         ( dmm() == other.dmm() );
}

bool operator ==(const rave::Covariance4D & lhs, const rave::Covariance4D & rhs)
{
  return lhs.equal( rhs );
}

} // namespace rave
