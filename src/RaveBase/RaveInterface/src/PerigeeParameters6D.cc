#include "RaveBase/RaveInterface/rave/PerigeeParameters6D.h"

#include <iomanip>

namespace rave
{

PerigeeParameters6D::PerigeeParameters6D(
  float rho, float theta, float phip, float epsilon, float zp,
  float mass) :
    PerigeeParameters5D(rho, theta, phip, epsilon, zp), theMass(mass)
{
}

PerigeeParameters6D::PerigeeParameters6D(
  const PerigeeParameters5D & fiveD, float mass) :
    PerigeeParameters5D(fiveD), theMass(mass)
{
}

PerigeeParameters6D::PerigeeParameters6D() :
    PerigeeParameters5D(), theMass(0.)
{
}

PerigeeParameters6D::~PerigeeParameters6D()
{
}

float PerigeeParameters6D::mass() const
{
  return theMass;
}

float PerigeeParameters6D::energy() const
{
  double cottheta = 1./tan(theTheta);
  return sqrt( 1./(theRho*theRho) * (1. + cottheta*cottheta) + theMass*theMass );
}

bool PerigeeParameters6D::equal(const PerigeeParameters6D& other) const
{
  return (mass() == other.mass()) && PerigeeParameters5D::equal(other);
}

bool operator== (const rave::PerigeeParameters6D & lhs, const rave::PerigeeParameters6D & rhs)
{
  return lhs.equal(rhs);
}

}
