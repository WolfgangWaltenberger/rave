#include "RaveBase/RaveInterface/rave/PerigeeCovariance5D.h"

namespace rave
{

PerigeeCovariance5D::PerigeeCovariance5D(
  float drr, float drt, float drp, float dre, float drz, float dtt,
  float dtp, float dte, float dtz, float dpp, float dpe, float dpz,
  float dee, float dez, float dzz) :
    PerigeeCovariance3D(drr, drt, drp, dtt, dtp, dpp),
    thedre(dre), thedrz(drz), thedte(dte), thedtz(dtz),
    thedpe(dpe), thedpz(dpz), thedee(dee), thedez(dez),
    thedzz(dzz)
{
}

PerigeeCovariance5D::PerigeeCovariance5D(
  const PerigeeCovariance3D & momentumerror,
  float dre, float drz, float dte, float dtz, float dpe, float dpz, 
  float dee, float dez, float dzz) :
    PerigeeCovariance3D(momentumerror),
    thedre(dre), thedrz(drz), thedte(dte), thedtz(dtz),
    thedpe(dpe), thedpz(dpz), thedee(dee), thedez(dez),
    thedzz(dzz)
{
}

PerigeeCovariance5D::PerigeeCovariance5D() :
    PerigeeCovariance3D(),
    thedre(0.), thedrz(0.), thedte(0.), thedtz(0.),
    thedpe(0.), thedpz(0.), thedee(0.), thedez(0.),
    thedzz(0.)
{
}

float PerigeeCovariance5D::dre() const
{
  return thedre;
}

float PerigeeCovariance5D::drz() const
{
  return thedrz;
}

float PerigeeCovariance5D::dte() const
{
  return thedte;
}

float PerigeeCovariance5D::dtz() const
{
  return thedtz;
}

float PerigeeCovariance5D::dpe() const
{
  return thedpe;
}

float PerigeeCovariance5D::dpz() const
{
  return thedpz;
}

float PerigeeCovariance5D::dee() const
{
  return thedee;
}

float PerigeeCovariance5D::dez() const
{
  return thedez;
}

float PerigeeCovariance5D::dzz() const
{
  return thedzz;
}

} // namespace rave
