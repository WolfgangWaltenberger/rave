#include "rave/KinematicVertex.h"

namespace rave
{

KinematicVertex::KinematicVertex( const BasicKinematicVertex & o ) :
    Base( o.clone() )
{}

KinematicVertex::KinematicVertex() :
    Base( new BasicKinematicVertex() )
{}

const rave::Point3D & KinematicVertex::position() const
{
  return data().position();
}

const rave::Covariance3D & KinematicVertex::error() const
{
  return data().error();
}

boost::weak_ptr< rave::BasicKinematicTree > KinematicVertex::correspondingTree() const
{
  return data().correspondingTree();
}

float KinematicVertex::ndf() const
{
  return data().ndf();
}

float KinematicVertex::chiSquared() const
{
  return data().chiSquared();
}

int KinematicVertex::id() const
{
  return data().id();
}

bool KinematicVertex::isValid() const
{
  return data().isValid();
}

void KinematicVertex::unlink()
{
  sharedData().unlink();
}

}
