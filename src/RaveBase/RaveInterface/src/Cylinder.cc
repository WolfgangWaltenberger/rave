#include "rave/Cylinder.h"

namespace ravesurf {
Cylinder::Cylinder ( const rave::Point3D & pos, const rave::Point3D & normal,
    float radius )
  : pos_ ( pos ), norm_ ( normal )
{}

rave::Point3D Cylinder::position() const
{
  return pos_;
}

rave::Vector3D Cylinder::axis() const
{
  return norm_;
}

float Cylinder::radius() const
{
  return radius_;
}

}
