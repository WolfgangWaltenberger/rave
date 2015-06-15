#include "RaveBase/RaveInterface/rave/Point3D.h"

namespace rave
{

Point3D::Point3D( float x, float y, float z ) :
  thex(x), they(y), thez(z) {}
  
Point3D::Point3D() :
  thex(0.), they(0.), thez(0.) {}

float Point3D::x() const
{
  return thex;
}

float Point3D::y() const
{
  return they;
}

float Point3D::z() const
{
  return thez;
}

float Point3D::mag2() const
{
  return thex*thex+they*they+thez*thez;
}

float Point3D::mag() const
{
  return sqrt(mag2());
}

float Point3D::perp2() const
{
  return thex*thex+they*they;
}

float Point3D::perp() const
{
  return sqrt ( perp2() );
}

bool Point3D::equal(const Point3D & other) const
{
  return ( x() == other.x() ) && ( y() == other.y() ) && ( z() == other.z() );
}

void Point3D::add(const Point3D & other)
{
  thex = thex + other.x();
  they = they + other.y();
  thez = thez + other.z();
}

void Point3D::multiply(double s)
{
  thex = thex * s;
  they = they * s;
  thez = thez * s;
}

void Point3D::subtract(const Point3D & other)
{
  thex = thex - other.x();
  they = they - other.y();
  thez = thez - other.z();
}

bool operator==( const rave::Point3D & lhs, const rave::Point3D & rhs)
{
  return lhs.equal(rhs);
}

rave::Point3D & operator+=( rave::Point3D & lhs, const rave::Point3D & rhs)
{
  lhs.add(rhs);
  return lhs;
}

rave::Point3D & operator-=( rave::Point3D & lhs, const rave::Point3D & rhs)
{
  lhs.subtract(rhs);
  return lhs;
}

rave::Point3D & operator*=( rave::Point3D & lhs, double s )
{
  lhs.multiply ( s );
  return lhs;
}

} // namespace rave
