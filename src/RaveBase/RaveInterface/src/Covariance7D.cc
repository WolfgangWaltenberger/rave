#include "RaveBase/RaveInterface/rave/Covariance7D.h"

#include <iomanip>

namespace rave
{

Covariance7D::Covariance7D ( float dxx, float dxy, float dxz,
                             float dyy, float dyz, float dzz,
                             float dxpx, float dxpy, float dxpz,
                             float dypx, float dypy, float dypz,
                             float dzpx, float dzpy, float dzpz,
                             float dpxpx, float dpxpy, float dpxpz,
                             float dpypy, float dpypz, float dpzpz,
                             float dxm, float dym, float dzm,
                             float dpxm, float dpym, float dpzm,
                             float dmm ) :
    Covariance6D ( dxx, dxy, dxz, dyy, dyz, dzz,
                   dxpx, dxpy, dxpz, dypx, dypy, dypz, dzpx, dzpy, dzpz,
                   dpxpx, dpxpy, dpxpz, dpypy, dpypz, dpzpz ),
    thedxm ( dxm ), thedym ( dym ), thedzm ( dzm ),
    thedpxm ( dpxm ), thedpym ( dpym ), thedpzm ( dpzm ), thedmm ( dmm )
{}

Covariance7D::Covariance7D ( Covariance6D space_error,
                             float dxm, float dym, float dzm,
                             float dpxm, float dpym, float dpzm,
                             float dmm ) :
    Covariance6D ( space_error ),
    thedxm ( dxm ), thedym ( dym ), thedzm ( dzm ),
    thedpxm ( dpxm ), thedpym ( dpym ), thedpzm ( dpzm ), thedmm ( dmm )
{}

Covariance7D::Covariance7D() :

    Covariance6D(), thedxm ( 0. ), thedym ( 0. ), thedzm ( 0. ),
    thedpxm ( 0. ), thedpym ( 0. ), thedpzm ( 0. ), thedmm ( 0. )
{}

float Covariance7D::dxm() const
{
  return thedxm;
}

float Covariance7D::dym() const

{
  return thedym;
}

float Covariance7D::dzm() const

{
  return thedzm;
}

float Covariance7D::dpxm() const

{
  return thedpxm;
}

float Covariance7D::dpym() const

{
  return thedpym;
}

float Covariance7D::dpzm() const

{
  return thedpzm;
}

float Covariance7D::dmm() const

{
  return thedmm;
}

bool Covariance7D::equal ( const Covariance7D & other ) const

{
  return Covariance6D::equal ( other ) &&
         ( dxm() == other.dxm() ) &&
         ( dym() == other.dym() ) &&
         ( dzm() == other.dzm() ) &&
         ( dpxm() == other.dpxm() ) &&
         ( dpym() == other.dpym() ) &&
         ( dpzm() == other.dpzm() ) &&
         ( dmm() == other.dmm() );
}

bool operator== ( const rave::Covariance7D & lhs, const rave::Covariance7D & rhs )
{
  return lhs.equal ( rhs );
}

} // namespace rave
