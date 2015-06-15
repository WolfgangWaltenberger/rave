#include "RaveBase/RaveInterface/rave/Vector6D.h"

namespace rave
{

Vector6D::Vector6D ( float x, float y, float z,
                     float px, float py, float pz ) :
    thex ( x ), they ( y ), thez ( z ), thepx ( px ), thepy ( py ), thepz ( pz )
{}

Vector6D::Vector6D ( const Point3D & position, const Vector3D & momentum ) :
    thex ( position.x() ), they( position.y() ), thez( position.z() ),
    thepx ( momentum.x() ), thepy ( momentum.y() ), thepz( momentum.z() )
{}

Vector6D::Vector6D() :
    thex ( 0. ), they ( 0. ), thez ( 0. ), thepx ( 0. ), thepy ( 0. ), thepz ( 0. )
{}


Vector3D Vector6D::momentum() const
{
  return Vector3D ( thepx, thepy, thepz );
}

Point3D Vector6D::position() const
{
  return Point3D ( thex, they, thez );
}

float Vector6D::x() const
{
  return thex;
}

float Vector6D::y() const
{
  return they;
}

float Vector6D::z() const
{
  return thez;
}

float Vector6D::px() const
{
  return thepx;
}

float Vector6D::py() const
{
  return thepy;
}

float Vector6D::pz() const
{
  return thepz;
}

bool Vector6D::equal(const Vector6D & other) const
{
  return ( momentum() == other.momentum() ) && ( position() == other.position() );
}

void Vector6D::addMomentum ( const Vector3D & other )
{
  thepx=thepx+other.x();
  thepy=thepy+other.y();
  thepz=thepz+other.z();
}

bool operator==(const rave::Vector6D & lhs, const rave::Vector6D & rhs)
{
  return lhs.equal(rhs);
}

} // namespace rave

