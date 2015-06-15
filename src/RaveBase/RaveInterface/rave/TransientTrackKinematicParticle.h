//
// C++ Interface: TransientTrackKinematicParticle
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVETRANSIENTTRACKKINEMATICPARTICLE_H
#define RAVETRANSIENTTRACKKINEMATICPARTICLE_H

#include <rave/KinematicParticle.h>
#include <rave/BasicTransientTrackKinematicParticle.h>

namespace rave {

/**
 * @brief The input data structure for a particle to be used in the kinematic
 *        fit.
 * @ingroup Rave
 * @author Fabian Moser <fabian@hephy.oeaw.ac.at>
 *
 * Creating instances of TransientTrackKinematicParticle is the only way to 
 * generate the data members required by the KinematicTreeFactory to use
 * Rave's kinematic fitting possibilities.
 */

class RaveDllExport TransientTrackKinematicParticle :
      public KinematicParticle
{
  public:
    friend class BasicTransientTrackKinematicParticle;

    TransientTrackKinematicParticle(
      const rave::Track & initialTrack, const float & massGuess,
      const float & massSigma, const float & chiSquared,
      const float & degreesOfFr, boost::any origin = boost::any(),
      std::string tag = "" );

    TransientTrackKinematicParticle(
      const rave::Track & initialTrack, const float & massGuess,
      const float & massSigma, boost::any origin = boost::any(),
      std::string tag = "" );

    TransientTrackKinematicParticle(
      const rave::Vector7D & state, const rave::Covariance7D & error,
      const rave::Charge & charge, const float & chiSquared,
      const float & degreesOfFr, boost::any origin = boost::any(),
      std::string tag = "" );

    TransientTrackKinematicParticle(
      const rave::Vector7D & state, const rave::PerigeeCovariance6D & error,
      const rave::Charge & charge, 
      const double & chiSquared, const double & degreesOfFr, 
      const rave::Point3D & referencePoint = rave::Point3D(0.,0.,0.),
      boost::any origin = boost::any(),
      std::string tag = "" );

    TransientTrackKinematicParticle(
      const rave::PerigeeParameters6D & state, 
      const rave::PerigeeCovariance6D & error,
      const rave::Charge & charge, const double & chiSquared, 
      const double & degreesOfFr,
      const rave::Point3D & referencePoint = rave::Point3D(0.,0.,0.),
      boost::any origin = boost::any(), std::string tag = "" );

    TransientTrackKinematicParticle();

    TransientTrackKinematicParticle closestToTransversePoint(
        const double & x = 0., const double & y = 0.) const;

  protected:
    TransientTrackKinematicParticle( const rave::BasicTransientTrackKinematicParticle & );
};

}

#endif
