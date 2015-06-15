#include "rave/Plane.h"

namespace ravesurf {
Plane::Plane ( const rave::Point3D & pos, const rave::Point3D & normal )
  : pos_ ( pos ), norm_ ( normal )
{}

rave::Point3D Plane::position() const
{
  return pos_;
}

rave::Vector3D Plane::normalVector() const
{
  return norm_;
}

}
