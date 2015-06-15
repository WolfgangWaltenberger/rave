#include "RaveBase/RaveEngine/interface/TaggedTransientTrackKinematicParticle.h"

TaggedTransientTrackKinematicParticle::TaggedTransientTrackKinematicParticle (
  const KinematicState & kineState, float & chiSquared, float & degreesOfFr,
  KinematicConstraint * lastConstraint,
  ReferenceCountingPointer< KinematicParticle > previousParticle,
  KinematicStatePropagator * pr,
  const boost::shared_ptr< reco::TransientTrack > initialTrack ) :
    TransientTrackKinematicParticle ( kineState, chiSquared, degreesOfFr,
                                      lastConstraint, previousParticle, pr,
                                      initialTrack.get() ),
    theInitialTrack( initialTrack )
{}

TaggedTransientTrackKinematicParticle::~ TaggedTransientTrackKinematicParticle()
{}

rave::TransientTrackKinematicParticle TaggedTransientTrackKinematicParticle::getTag() const
{
  return theTag;
}

void TaggedTransientTrackKinematicParticle::setTag ( const rave::TransientTrackKinematicParticle & tag )
{
  theTag = tag;
}
