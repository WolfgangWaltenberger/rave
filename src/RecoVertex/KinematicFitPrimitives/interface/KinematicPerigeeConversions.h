#ifndef KinematicPerigeeConversions_H
#define KinematicPerigeeConversions_H

#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParameters.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicState.h"
#include "RecoVertex/KinematicFitPrimitives/interface/ExtendedPerigeeTrajectoryParameters.h"
#include "RecoVertex/KinematicFitPrimitives/interface/ExtendedPerigeeTrajectoryError.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "RecoVertex/KinematicFitPrimitives/interface/Matrices.h"

/**
 * Helper class to simplify parameters 
 * conversions between kinematic
 * and extended perigee parametrization
 *
 * Kirill Prokofiev, August 2003
 */
class KinematicPerigeeConversions
{
public:

 KinematicPerigeeConversions()
 {}
 
 /**
  * Calculates the perigee parameters from a cartesian state.
  *
  * @param state  The cartesian state
  * @param point  The perigee reference pointpr
  */
 ExtendedPerigeeTrajectoryParameters extendedPerigeeFromKinematicParameters
 	(const KinematicState& state, const GlobalPoint& point) const;

 /**
  * Calculates the perigee parameters from cartesian parameters.
  *
  * @param parameters  The cartesian state
  * @param charge  The charge of the particle
  * @param refernecePoint  The perigee reference pointpr
   */
 ExtendedPerigeeTrajectoryParameters extendedPerigeeFromKinematicParameters
     (const KinematicParameters & parameters, const TrackCharge & charge, 
      const GlobalPoint & referencePoint, const MagneticField* field) const;
 
 /**
  * Converts the cartesian covariance matrix to the perigee one.
  * @author Fabian Moser <fabiamos@gmail.com>
  * @param error  The cartesian covariance matrix
  * @param state  The associated state necessary to build the jacobian
  * @param charge The charge of the particle
  * @param point  The perigee reference pointpr
  */
 ExtendedPerigeeTrajectoryError extendedPerigeeErrorFromKinematicParametersError
     (const KinematicParametersError& error,
      const KinematicParameters& parameters,
      const TrackCharge& charge,
      const GlobalPoint& referencePoint,
      const MagneticField* field) const;
 
 /**
  * Converts the perigee parameters to the cartesian ones.
  *
  * @param pe The perigee parameters
  * @param point The perigee reference point
  * @param field The magnetic field
  */
 KinematicParameters kinematicParametersFromExPerigee
 	(const ExtendedPerigeeTrajectoryParameters& pr,	const GlobalPoint& point,
	 const MagneticField* field) const;

 /**
  * Converts the perigee covariance matrix to the cartesian one
  * @author Fabian Moser <fabiamos@gmail.com>
  * @param error The perigee covariance matrix
  * @param state The associated perigee state necessary to build the jacobian
  * @param field The magnetic field
  */
 KinematicParametersError kinematicParametersErrorFromExPerigee
     (const ExtendedPerigeeTrajectoryError& error,
      const ExtendedPerigeeTrajectoryParameters& parameters,
      const GlobalPoint& referencePoint,
      const MagneticField* field) const;
 
 KinematicState kinematicState(const AlgebraicVector4& momentum,
	const GlobalPoint& referencePoint, const TrackCharge& charge,
	const AlgebraicSymMatrix77& theCovarianceMatrix, const MagneticField* field) const;
				 
    /**
     * Cartesian (px,py,px,m) from extended perigee
     */ 
 AlgebraicVector4 momentumFromPerigee(const AlgebraicVector4& momentum,
	const GlobalPoint& referencePoint, const TrackCharge& ch,
	const MagneticField* field) const;
				     
private:
  /**
   * Jacobians of tranformations from the parametrixation
   * (x, y, z, transverse curvature, theta, phi,m) to kinematic
   *  parameters
   */
  AlgebraicMatrix77 jacobianParameters2Kinematic(const AlgebraicVector4& momentum, 
	const GlobalPoint& referencePoint, const TrackCharge& charge,
	const MagneticField* field) const;


  /**
   * Jacobian to allow error propagation from perigee to cartesian parameters
   * @author Fabian Moser <fabiamos@gmail.com>
   * @param parameters The associated set of perigee parameters
   * @param referencePoint The reference point of the perigee parameters
   * @param field The magnetic field, will be evaluated at referencePoint
   */
  AlgebraicMatrix76 jacobianPerigeeToCartesian( 
      const ExtendedPerigeeTrajectoryParameters& parameters,
      const GlobalPoint& referencePoint,
      const MagneticField* field) const;

  /**
   * Jacobian to allow error propagation from cartesian to perigee parameters
   * @author Fabian Moser <fabiamos@gmail.com>
   * @param parameters The associated set of cartsian parameters
   * @param charge The charge associated with the particle
   * @param field The magnetic field, will be evaluated at referencePoint
   */
  AlgebraicMatrix67 jacobianCartesianToPerigee( 
    const KinematicParameters& parameters,
    const TrackCharge& charge,
    const GlobalPoint& referencePoint,
    const MagneticField* field) const;
};

#endif
