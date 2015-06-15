#include "RaveBase/RaveInterface/rave/PerigeeCovariance3D.h"

namespace rave {

PerigeeCovariance3D::PerigeeCovariance3D(
  float drr, float drt, float drp, float dtt, float dtp, float dpp) :
    thedrr(drr), thedrt(drt), thedrp(drp), thedtt(dtt),
    thedtp(dtp), thedpp(dpp)
{}

PerigeeCovariance3D::PerigeeCovariance3D() :
    thedrr(0.), thedrt(0.), thedrp(0.), thedtt(0.),
    thedtp(0.), thedpp(0.)
{}

float PerigeeCovariance3D::drr() const
{
  return thedrr;
}

float PerigeeCovariance3D::drt() const
{
  return thedrt;
}

float PerigeeCovariance3D::drp() const
{
  return thedrp;
}

float PerigeeCovariance3D::dtt() const
{
  return thedtt;
}

float PerigeeCovariance3D::dtp() const
{
  return thedtp;
}

float PerigeeCovariance3D::dpp() const
{
  return thedpp;
}

} // namespace rave
