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
#ifndef RAVEBASICTRANSIENTTRACKKINEMATICPARTICLE_H
#define RAVEBASICTRANSIENTTRACKKINEMATICPARTICLE_H

#include <rave/Track.h>
#include <rave/KinematicParticle.h>

namespace rave
{

/**
 * @class BasicTransientTrackKinematicParticle represents a particle created
 * from or at least associated with a rave::TransientTrack.
 *
 * As the other rave::KinematicParticle classes, this cannot be created
 * directly by the user. But since this class acts as an input data structure,
 * the user can create instances through the respective builder called
 * rave::KinematicParticleBuilder
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */
class RaveDllExport BasicTransientTrackKinematicParticle :
      public BasicKinematicParticle
{
  public:
    typedef RaveProxyBase< BasicTransientTrackKinematicParticle, CopyUsingNew<BasicTransientTrackKinematicParticle> > Proxy;

    friend class BasicKinematicTree;

    BasicTransientTrackKinematicParticle (
      const rave::Track & initialTrack, const float & massGuess,
      const float & massSigma, const float & chiSquared,
      const float & degreesOfFr, boost::any origin = boost::any(),
      std::string tag = "" );

    BasicTransientTrackKinematicParticle (
      const rave::Track & initialTrack, const float & massGuess,
      const float & massSigma, boost::any origin = boost::any(),
      std::string tag = "" );

    BasicTransientTrackKinematicParticle (
      const rave::Vector7D & state, const rave::Covariance7D & error,
      const rave::Charge & charge, const float & chiSquared,
      const float & degreesOfFr, boost::any origin = boost::any(),
      std::string tag = "" );

    BasicTransientTrackKinematicParticle (
      const rave::Vector7D & state, const rave::PerigeeCovariance6D & error,
      const rave::Charge & charge, const double & chiSquared,
      const double & degreesOfFr, 
      const rave::Point3D & referencePoint = rave::Point3D(0.,0.,0.),
      boost::any origin = boost::any(), std::string tag = "" );

    BasicTransientTrackKinematicParticle (
      const rave::PerigeeParameters6D & state,
      const rave::PerigeeCovariance6D & error,
      const rave::Charge & charge,
      const double & chiSquared, const double & degreesOfFr,
      const rave::Point3D & referencePoint = rave::Point3D(0.,0.,0.),
      boost::any origin = boost::any(), std::string tag = "" );

    BasicTransientTrackKinematicParticle();
    virtual ~BasicTransientTrackKinematicParticle();

    virtual BasicTransientTrackKinematicParticle * clone () const;

    TransientTrackKinematicParticle closestToTransversePoint(
      const double & x = 0., const double & y = 0.) const;

  private:
    /**
     * The constructor initializes the particle data structure with the given
     * values. The last constraint and the previous particle are used.
     * @param particle The particle to be interfaced by the new instance
     * @param origin A backlink possibility e.g. for linking to "original" data
     * @param tag A string tag to eventually associate additional information
     */
    BasicTransientTrackKinematicParticle (
      boost::any particle, boost::any origin = boost::any(),
      std::string tag = "" );

    /**
     * The constructor initializes the particle data structure with the given
     * values. The last constraint and the previous particle are used.
     * @param id A Rave internal and unique id
     * @param particle The particle to be interfaced by the new instance
     * @param origin A backlink possibility e.g. for linking to "original" data
     * @param tag A string tag to eventually associate additional information
     */
    BasicTransientTrackKinematicParticle (
      int id, boost::any particle, boost::any origin = boost::any(),
      std::string tag = "" );

};

}

#endif
