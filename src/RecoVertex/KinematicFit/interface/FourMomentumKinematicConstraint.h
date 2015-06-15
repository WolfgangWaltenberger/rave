#ifndef FourMomentumKinematicConstraint_H
#define FourMomentumKinematicConstraint_H

#include "RecoVertex/KinematicFitPrimitives/interface/KinematicConstraint.h"
#include "RecoVertex/KinematicFitPrimitives/interface/ParticleMass.h"

/**
 * 4-Momentum constraint class provides a way to compute the
 * matrix of derivatives and the vector of values for 4-Momentum
 * constraint for given set of KinematicParticles.
 * 
 * Kirill Prokofiev March 2003
 * MultiState version: July 2004
 *
 * Fabian Moser 2008:
 *  - Implementation for MultiState situation
 *  - Added switch to use energy instead of mass in the kinematic fit
 */

class FourMomentumKinematicConstraint : public KinematicConstraint
{

public:

/**
 * Constructor with desired 4-momentum vector  and 
 * vector of deviations to be used forcovariance matrix as
 * arguments
 */
 FourMomentumKinematicConstraint(const AlgebraicVector& momentum,
                                 const AlgebraicVector& deviation,
                                 const bool mass_is_energy = false);

/**
 * Vector of values and  matrix of derivatives
 * calculated at given  7*NumberOfStates expansion point
 */  
virtual pair<AlgebraicVector,AlgebraicVector> value(const AlgebraicVector& exPoint) const;
 
virtual pair<AlgebraicMatrix, AlgebraicVector> derivative(const AlgebraicVector& exPoint) const;


/**
 * Vector of values and matrix of derivatives calculated using 
 * current state as an expansion point
 */  
virtual pair<AlgebraicMatrix, AlgebraicVector> derivative(const vector<RefCountedKinematicParticle> par) const;

virtual pair<AlgebraicVector, AlgebraicVector> value(const vector<RefCountedKinematicParticle> par) const;

 
/**
 * Returns number of constraint equations used for fitting. Method is relevant for proper NDF
 * calculations.
 */ 
virtual int numberOfEquations() const;

virtual AlgebraicVector deviations(int nStates) const;

virtual FourMomentumKinematicConstraint * clone() const
 {return new FourMomentumKinematicConstraint(*this);}

private:

AlgebraicVector mm;
AlgebraicVector dd;
bool massIsEnergy;

};
#endif
