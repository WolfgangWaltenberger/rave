#include "RaveBase/RaveInterface/rave/PerigeeParameters3D.h"

namespace rave
{

PerigeeParameters3D::PerigeeParameters3D(
  float rho, float theta, float phip) :
    theRho(rho), theTheta(theta), thePhiP(phip)
{
}

PerigeeParameters3D::PerigeeParameters3D() :
    theRho(0.), theTheta(0.), thePhiP(0.)
{}

float PerigeeParameters3D::rho() const
{
  return theRho;
}

float PerigeeParameters3D::theta() const
{
  return theTheta;
}

float PerigeeParameters3D::phip() const
{
  return thePhiP;
}

bool PerigeeParameters3D::equal(const PerigeeParameters3D & other) const
{
  return (rho() == other.rho()) &&
         (theta() == other.theta()) &&
         (phip() == other.phip());
}

bool operator== (const rave::PerigeeParameters3D & lhs, const rave::PerigeeParameters3D & rhs)
{
  return lhs.equal(rhs);
}

} // namespace rave
