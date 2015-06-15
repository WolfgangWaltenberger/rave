//
// C++ Interface: TaggedTransientTrackKinematicParticle
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KINEMATICSTAGGEDTRANSIENTTRACKKINEMATICPARTICLE_H
#define KINEMATICSTAGGEDTRANSIENTTRACKKINEMATICPARTICLE_H

#include "RaveBase/RaveInterface/rave/TransientTrackKinematicParticle.h"

#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"

#include "boost/shared_ptr.hpp"

/**
 * This class extends the TransientTrackKinematicParticle with a
 * reference to the corresponding rave::TransientTrackKinematicParticle
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class TaggedTransientTrackKinematicParticle :
      public TransientTrackKinematicParticle
{
  public:
    friend class rave::TransientTrackKinematicParticle;

    TaggedTransientTrackKinematicParticle (
      const KinematicState& kineState, float& chiSquared,
      float& degreesOfFr, KinematicConstraint * lastConstraint,
      ReferenceCountingPointer<KinematicParticle> previousParticle,
      KinematicStatePropagator * pr,
      const boost::shared_ptr< reco::TransientTrack > initialTrack =
        boost::shared_ptr< reco::TransientTrack >() );

    virtual ~TaggedTransientTrackKinematicParticle();

    rave::TransientTrackKinematicParticle getTag() const;

  private:
    void setTag ( const rave::TransientTrackKinematicParticle & tag );

    const boost::shared_ptr< reco::TransientTrack > theInitialTrack;
    rave::TransientTrackKinematicParticle theTag;

};

#endif
