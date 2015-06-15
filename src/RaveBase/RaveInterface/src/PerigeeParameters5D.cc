#include "RaveBase/RaveInterface/rave/PerigeeParameters5D.h"

namespace rave {

PerigeeParameters5D::PerigeeParameters5D(
  float rho, float theta, float phip, float epsilon, float zp) :
    PerigeeParameters3D(rho, theta, phip), theEpsilon(epsilon), theZP(zp)
{
}

PerigeeParameters5D::PerigeeParameters5D(
  const PerigeeParameters3D & momentum, float epsilon, float zp) :
    PerigeeParameters3D(momentum), theEpsilon(epsilon), theZP(zp)
{
}

PerigeeParameters5D::PerigeeParameters5D() :
    PerigeeParameters3D(), theEpsilon(0.), theZP(0.)
{
}

float PerigeeParameters5D::epsilon() const
{
  return theEpsilon;
}

float PerigeeParameters5D::zp() const
{
  return theZP;
}

bool PerigeeParameters5D::equal(const PerigeeParameters5D & other) const
{
  return (epsilon() == other.epsilon()) &&
         (zp() == other.zp()) &&
         PerigeeParameters3D::equal(other);
}

bool operator== (const rave::PerigeeParameters5D & lhs, const rave::PerigeeParameters5D & rhs)
{
  return lhs.equal(rhs);
}

} // namespace rave
