#include "RaveBase/RaveInterface/rave/PerigeeCovariance6D.h"

#include <iomanip>

namespace rave {

PerigeeCovariance6D::PerigeeCovariance6D(
  float drr, float drt, float drp, float dre, float drz, float drm,
  float dtt, float dtp, float dte, float dtz, float dtm, float dpp,
  float dpe, float dpz, float dpm, float dee, float dez, float dem,
  float dzz, float dzm, float dmm) :
    PerigeeCovariance5D(drr, drt, drp, dre, drz, dtt, dtp, dte, dtz, dpp,
                        dpe, dpz, dee, dez, dzz),
    thedrm(drm), thedtm(dtm), thedpm(dpm), thedem(dem), thedzm(dzm),
    thedmm(dmm)
{
}

PerigeeCovariance6D::PerigeeCovariance6D(
  const PerigeeCovariance5D & fiveError,
  float drm, float dtm, float dpm, float dem, float dzm, float dmm) :
    PerigeeCovariance5D(fiveError),
    thedrm(drm), thedtm(dtm), thedpm(dpm), thedem(dem), thedzm(dzm),
    thedmm(dmm)
{
}

PerigeeCovariance6D::PerigeeCovariance6D() :
    PerigeeCovariance5D(),
    thedrm(0.), thedtm(0.), thedpm(0.), thedem(0.), thedzm(0.), thedmm(0.)
{
}

float PerigeeCovariance6D::drm() const
{
  return thedrm;
}

float PerigeeCovariance6D::dtm() const
{
  return thedtm;
}

float PerigeeCovariance6D::dpm() const
{
  return thedpm;
}

float PerigeeCovariance6D::dem() const
{
  return thedem;
}

float PerigeeCovariance6D::dzm() const
{
  return thedzm;
}

float PerigeeCovariance6D::dmm() const
{
  return thedmm;
}

} // namespace rave
