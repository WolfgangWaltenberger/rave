#include "RaveBase/RaveInterface/rave/BasicVirtualKinematicParticle.h"

#include "RecoVertex/KinematicFitPrimitives/interface/VirtualKinematicParticle.h"

#include "boost/assert.hpp"
#include "boost/cast.hpp"

namespace rave
{

BasicVirtualKinematicParticle::BasicVirtualKinematicParticle() :
    BasicKinematicParticle()
{}

BasicVirtualKinematicParticle::~ BasicVirtualKinematicParticle()
{}

BasicVirtualKinematicParticle * BasicVirtualKinematicParticle::clone() const
{
  return new BasicVirtualKinematicParticle ( *this );
}

BasicVirtualKinematicParticle::BasicVirtualKinematicParticle (
  boost::any particle, boost::any origin, std::string tag ) :
    BasicKinematicParticle ( particle, origin, tag )
{
  BOOST_ASSERT ( typeid ( ::RefCountedKinematicParticle ) == particle.type() );
}

BasicVirtualKinematicParticle::BasicVirtualKinematicParticle (
  int id, boost::any particle, boost::any origin, std::string tag ) :
    BasicKinematicParticle ( id, particle, origin, tag )
{
  BOOST_ASSERT ( typeid ( ::RefCountedKinematicParticle ) == particle.type() );
}

}
