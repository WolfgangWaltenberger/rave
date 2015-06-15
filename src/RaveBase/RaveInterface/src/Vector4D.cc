#include "RaveBase/RaveInterface/rave/Vector4D.h"

#include <iostream>
using namespace std;

namespace rave
{

Vector4D::Vector4D ( float x, float y, float z, float m ) :
    Vector3D ( x, y, z ), them ( m )
{}

Vector4D::Vector4D ( rave::Vector3D v, float m ) :
    Vector3D ( v ), them ( m )
{}

Vector4D::Vector4D() :
    Vector3D(), them ( 0. )
{}

float Vector4D::m() const
{
  return them;
}

bool Vector4D::equal ( const rave::Vector4D & o ) const
{
  return Vector3D::equal ( o ) && ( m() == o.m() );
}

bool operator == ( const rave::Vector4D & lhs, const rave::Vector4D & rhs )
{
  return lhs.equal ( rhs );
}

float Vector4D::energy() const
{
  return sqrt ( them*them + mag2() );
}

void Vector4D::add(const Vector4D & other)
{
  float Eother=other.energy();
  float Eme=energy();
  thex = thex + other.x();
  they = they + other.y();
  thez = thez + other.z();
  float Enew = Eme + Eother;
  float m2 = Enew * Enew - mag2();
  if (m2 > 0.)
  {
    them = sqrt ( m2 );
  } else {
    them = 0.;
  }
}

rave::Vector3D Vector4D::p3() const
{
  return rave::Vector3D ( *this );
}

void Vector4D::subtract(const Vector4D & other)
{
  thex = thex - other.x();
  they = they - other.y();
  thez = thez - other.z();
}

rave::Vector4D & operator+=( rave::Vector4D & lhs, const rave::Vector4D & rhs)
{
  lhs.add(rhs);
  return lhs;
}

rave::Vector4D & operator-=( rave::Vector4D & lhs, const rave::Vector4D & rhs)
{
  lhs.subtract(rhs);
  return lhs;
}

} // namespace rave

