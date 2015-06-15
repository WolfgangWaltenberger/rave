#include "rave/VirtualKinematicParticle.h"

namespace rave
{

VirtualKinematicParticle::VirtualKinematicParticle (
  const BasicVirtualKinematicParticle & o ) :
    KinematicParticle ( *(o.clone()) )
{}

VirtualKinematicParticle::VirtualKinematicParticle() :
    KinematicParticle ( BasicVirtualKinematicParticle () )
{}

}
