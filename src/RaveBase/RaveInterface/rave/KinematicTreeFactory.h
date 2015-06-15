//
// C++ Interface: KinematicTreeFactory
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEKINEMATICTREEFACTORY_H
#define RAVEKINEMATICTREEFACTORY_H

#include <rave/KinematicParticle.h>
#include <rave/KinematicTree.h>
#include <rave/ConstantMagneticField.h>
#include <rave/VacuumPropagator.h>
#include <rave/Ellipsoid3D.h>

namespace rave
{

/**
 * @class KinematicTreeFactory provides the interface for fitting a decay chain
 * from given particles.
 *
 * Together with KinematicParticleFactory, KinematicTreeFactory  is at the
 * heart of the Rave kinematic fit capabilities. While the first creates
 * particle representations out of track data, the second fits these particle
 * representations called KinematicParticles to a decay chain represented by
 * a KinematicTree instance.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */
class RaveDllExport KinematicTreeFactory
{
  public:
    KinematicTreeFactory ( const rave::MagneticField & f = ConstantMagneticField(),
                           const rave::Propagator & p = VacuumPropagator(),
                           int verbosity = 1 );

    KinematicTreeFactory ( const rave::MagneticField & f,
                           const rave::Propagator & p,
                           const rave::Ellipsoid3D & beamspot,
                           int verbosity = 1 );

    ~KinematicTreeFactory();

    /**
     * Uses KinematicParticleVertexFitter
     * @param particles Field of particles
     * @param beamspot make use of beamspot constraint for first vertex
     * @return The fitted tree
     */
    rave::KinematicTree useVertexFitter (
      std::vector< rave::KinematicParticle > particles,
      std::string parameters = "", bool beamspot=false ) const;

    /**
     * Does a constraint kinematic vertex fit using the KinematicConstrainedVertexFitter
     * @param particles Field of particles
     * @param constr Constraint
     * @param beamspot make use of beamspot constraint for first vertex
     * @param fnd String selecting the used LinearisationPointFinder one of
     * lms,
     * @return The fitted tree
     */
    rave::KinematicTree useVertexFitter (
      std::vector< rave::KinematicParticle > particles,
      rave::KinematicConstraint cs,
      std::string parameters = "", bool beamspot=false ) const;

    /**
     * Uses KinematicParticleFitter
     * @param trees
     * @param cs
     * @param parameters String selecting the fitter-updator pair.
     * @param beamspot make use of beamspot constraint for first vertex
     * @return The refitted trees
     */
    std::vector< rave::KinematicTree > useParticleFitter (
      std::vector< rave::KinematicTree > trees,
      rave::KinematicConstraint cs,
      std::string parameters = "ppf:lppf", bool beamspot=false ) const;

    /**
     * Generates single-particle trees from the given particles and 
     * uses KinematicParticleFitter on those trees returning again
     * only the topParticles of the resulting trees. 
     * @param particles The input particles
     * @param cs The constraint
     * @param parameters String selecting the fitter-updator pair.
     * @param beamspot make use of beamspot constraint for first vertex
     * @return The refitted particles
     */
    std::vector< rave::KinematicParticle > useParticleFitter (
      std::vector< rave::KinematicParticle > particles,
      rave::KinematicConstraint cs,
      std::string parameters = "ppf:lppf", bool beamspot=false ) const;
    
    /// returns the propagator that is used
    const rave::Propagator & getPropagator() const;

    /// returns the magnetic field
    const rave::MagneticField & getMagneticField() const;

    /// returns the beamspot constraint that is used
    const rave::Ellipsoid3D & getBeamspot() const;

    /// true, if the factory knows of a beamspot constraint
    bool hasBeamspot() const;

    int verbosity() const;

  private:
    void setup();
    /** unlink all objects in the to-be-unlinked containers
     */
    void unlink() const;
    void addToUnlinked ( const std::vector< rave::KinematicTree > & ) const;
    void addToUnlinked ( const std::vector< rave::KinematicParticle > & ) const;
    void addToUnlinked ( const rave::KinematicTree & ) const;
    void wipe() const;

    rave::MagneticField * theField;
    rave::Propagator * thePropagator;
    int theVerbosity;
    mutable std::vector< rave::KinematicTree > toBeUnlinkedTrees;
    mutable std::vector< rave::KinematicParticle > toBeUnlinkedParticles;
};

}

#endif
