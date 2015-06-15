//
// C++ Interface: KinematicConstraintBuilder
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEKINEMATICCONSTRAINTBUILDER_H
#define RAVEKINEMATICCONSTRAINTBUILDER_H

#include <rave/Vector3D.h>
#include <rave/Vector4D.h>
#include <rave/KinematicConstraint.h>
#include <rave/MultipleKinematicConstraint.h>

namespace rave
{

/**
 * @class KinematicConstraintBuilder allows the user to create kinematic
 * constraint objects of type rave::KinematicConstraint.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
*/
class RaveDllExport KinematicConstraintBuilder
{
  public:
    KinematicConstraintBuilder();
    virtual ~KinematicConstraintBuilder();

    rave::KinematicConstraint createBackToBackKinematicConstraint() const;

    /**
     * Creates a constraint to link the first two particles' masses to each 
     * other. To be used with the Vertex Fitter only.
     */
    rave::KinematicConstraint createEqualMassKinematicConstraint() const;

    rave::KinematicConstraint createFourMomentumKinematicConstraint (
      const rave::Vector4D & momentum,
      const rave::Vector4D & deviation,
      const bool mass_is_energy = false) const;

    rave::KinematicConstraint createMomentumKinematicConstraint (
      const rave::Vector3D & momentum,
      const rave::Vector3D & deviation ) const;

    rave::KinematicConstraint createPointingKinematicConstraint (
      const rave::Point3D & reference ) const;

    rave::KinematicConstraint createSimplePointingConstraint (
      const rave::Point3D & reference ) const;

    rave::KinematicConstraint createSmartPointingConstraint (
      const rave::Point3D & reference ) const;

    /**
     * Creates an instance of a mass constraint object to be assigned to a
     * particle or used as a parameter for a fitter.
     * @param mass The mass to which the fit should be constraint
     * @param sigma The width of the mass
     * @return A new instance if rave::KinematicConstraint representing the mass constraint
     */
    rave::KinematicConstraint createMassKinematicConstraint (
      const float & mass,
      const float sigma ) const;

    /**
     * Creates an instance of rave::MultipleKinematicConstraint, which provides
     * the user with the ability of bundling/combining constraints
     * @return A new instance of rave::MultipleKinematicConstraint
     */
    rave::MultipleKinematicConstraint createMultipleKinematicConstraint() const;

    rave::KinematicConstraint createVertexKinematicConstraint() const;

    rave::KinematicConstraint createTwoTrackMassKinematicConstraint (
      const float & mass ) const;

    rave::KinematicConstraint createMultiTrackMassKinematicConstraint (
      const float & mass, int nparticles ) const;
};

}

#endif
