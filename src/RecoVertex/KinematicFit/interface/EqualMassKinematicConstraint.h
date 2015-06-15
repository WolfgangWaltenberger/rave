#ifndef EqualMassKinematicConstraint_H
#define EqualMassKinematicConstraint_H

#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicState.h"
#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"
#include "RecoVertex/KinematicFitPrimitives/interface/ParticleMass.h"

/**
 * Class implementing the equal mass
 * of 2 tracks constraint.
 *
 * Warning: the tracks to constraint
 * should be 1st and 2nd from the
 * beginning of the vector.
 *
 */

class EqualMassKinematicConstraint :
      public MultiTrackKinematicConstraint
{
  public:
    EqualMassKinematicConstraint() {}

    /**
    * Returns a vector of values of constraint
    * equations at the point where the input
    * particles are defined.
    */
    virtual AlgebraicVector value ( const vector<KinematicState> & states,
                                    const GlobalPoint& point ) const;

    /**
     * Returns a matrix of derivatives of
     * constraint equations w.r.t.
     * particle parameters
     */
    virtual AlgebraicMatrix parametersDerivative ( const vector<KinematicState> & states,
        const GlobalPoint& point ) const;

    /**
     * Returns a matrix of derivatives of
     * constraint equations w.r.t.
     * vertex position
     */
    virtual AlgebraicMatrix positionDerivative ( const vector<KinematicState> & states,
        const GlobalPoint& point ) const;

    /**
     * Number of equations per track used for the fit
     */
    virtual int numberOfEquations() const;

    virtual EqualMassKinematicConstraint * clone() const
    {
      return new EqualMassKinematicConstraint ( *this );
    }
};
#endif
