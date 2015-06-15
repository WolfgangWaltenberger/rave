#include "RecoVertex/KinematicFit/interface/FourMomentumKinematicConstraint.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"


FourMomentumKinematicConstraint::FourMomentumKinematicConstraint(const AlgebraicVector& momentum,
                                                                 const AlgebraicVector& deviation,
                                                                 const bool mass_is_energy)
{
 if((momentum.num_row() != 4)||(deviation.num_row() != 4)) 
  throw VertexException("FourMomentumKinematicConstraint::vector of wrong size passed as 4-Momentum or Deviations");
 mm = momentum;
 AlgebraicVector deviation_l(7,0);
 massIsEnergy = mass_is_energy;

 deviation_l(6) = momentum(3);
 deviation_l(5) = momentum(2);
 deviation_l(4) = momentum(1);
  
 double mass_sq = momentum(4)*momentum(4) - momentum(3)*momentum(3)
                - momentum(2)*momentum(2) - momentum(1)*momentum(1); 

 if (mass_sq == 0.)
   throw VertexException("FourMomentumKinematicConstraint::the constraint vector passed corresponds to 0 mass");
//deviation for mass calculated from deviations
//of momentum
 deviation_l(7) = momentum(4)*momentum(4)*deviation(4)/mass_sq 
                + momentum(3)*momentum(3)*deviation(3)/mass_sq
		            + momentum(2)*momentum(2)*deviation(2)/mass_sq
		            + momentum(1)*momentum(1)*deviation(1)/mass_sq;
//mass sigma because of the energy 
 
 dd = deviation_l;
}

pair<AlgebraicVector,AlgebraicVector> FourMomentumKinematicConstraint::value(const AlgebraicVector& exPoint) const
{
 if (exPoint.num_row() == 0) 
   throw VertexException("FourMomentumKinematicConstraint::value requested for zero Linearization point");

//security check for extended cartesian parametrization 
 int inSize = exPoint.num_row(); 
 if ((inSize % 7) != 0) 
   throw VertexException("FourMomentumKinematicConstraint::linearization point has a wrong dimension");
 
 int nStates = inSize / 7;
 //if(nStates != 1) throw VertexException("FourMomentumKinematicConstraint::Multi particle refit is not supported in this version");

 AlgebraicVector pr = exPoint;
 AlgebraicVector vl(4,0);

 for (int i = 1; i <= vl.num_row(); i++)
   vl(i) = - mm(i);

 for (int i = 0; i < nStates; i++)
 {
   vl(1) += pr(i*7 + 4);
   vl(2) += pr(i*7 + 5);
   vl(3) += pr(i*7 + 6);
   if (massIsEnergy) {
     vl(4) += pr(i*7 + 7);
   } else {
     vl(4) += sqrt(pr(i*7 + 4)*pr(i*7 + 4) + 
                   pr(i*7 + 5)*pr(i*7 + 5) + 
                   pr(i*7 + 6)*pr(i*7 + 6) + 
                   pr(i*7 + 7)*pr(i*7 + 7));
   }
 }
 
 return pair<AlgebraicVector, AlgebraicVector>(vl, pr);
}
 
pair<AlgebraicMatrix, AlgebraicVector> FourMomentumKinematicConstraint::derivative(const AlgebraicVector& exPoint) const
{
 if (exPoint.num_row() == 0) 
   throw VertexException("FourMomentumKinematicConstraint::value requested for zero Linearization point");

//security check for extended cartesian parametrization 
 int inSize = exPoint.num_row(); 
 if ((inSize % 7) != 0) 
   throw VertexException("FourMomentumKinematicConstraint::linearization point has a wrong dimension");
 
 int nStates = inSize/7;
 //if (nStates != 1) throw VertexException("FourMomentumKinematicConstraint::Multi particle refit is not supported in this version");
 
 AlgebraicVector pr = exPoint;
 AlgebraicMatrix dr(4, 7*nStates, 0);
 
 for (int i = 0; i < nStates; i++)
 {
   dr(1,i*7 + 4) = 1.;
   dr(2,i*7 + 5) = 1.;
   dr(3,i*7 + 6) = 1.;
   
   if (massIsEnergy) {
     dr(4,i*7 + 7) = 1.;
   } else {
     double invenergy = 1./sqrt(pr(i*7 + 4)*pr(i*7 + 4) +
                                pr(i*7 + 5)*pr(i*7 + 5) +
                                pr(i*7 + 6)*pr(i*7 + 6) +
                                pr(i*7 + 7)*pr(i*7 + 7));
     dr(4,i*7 + 4) = pr(i*7 + 4)*invenergy;
     dr(4,i*7 + 5) = pr(i*7 + 5)*invenergy;
     dr(4,i*7 + 6) = pr(i*7 + 6)*invenergy;
     dr(4,i*7 + 7) = pr(i*7 + 7)*invenergy;
   }
 }

 return pair<AlgebraicMatrix, AlgebraicVector>(dr, pr);
}

pair<AlgebraicVector, AlgebraicVector> FourMomentumKinematicConstraint::value(const vector<RefCountedKinematicParticle> par) const
{
 int nStates = par.size();
 if (nStates == 0) 
   throw VertexException("FourMomentumKinematicConstraint::Empty vector of particles passed");
 //if (nStates != 1) throw VertexException("FourMomentumKinematicConstraint::Multi particle refit is not supported in this version");
 
 AlgebraicVector vl(4,0);
 AlgebraicVector pr(7*nStates, 0);
 
 for (int i = 1; i <= vl.num_row(); i++)
   vl(i) = - mm(i);

 for (int i = 0; i < nStates; i++)
 {
   pr.sub(i*7 + 1, asHepVector<7>(par[i]->currentState().kinematicParameters().vector()));
   vl(1) += pr(i*7 + 4);
   vl(2) += pr(i*7 + 5);
   vl(3) += pr(i*7 + 6);
   if (massIsEnergy) {
     vl(4) += pr(i*7 + 7);
   } else {
     vl(4) += sqrt(pr(i*7 + 4)*pr(i*7 + 4) + 
                   pr(i*7 + 5)*pr(i*7 + 5) + 
                   pr(i*7 + 6)*pr(i*7 + 6) + 
                   pr(i*7 + 7)*pr(i*7 + 7));
   }
 }
 
 return pair<AlgebraicVector,AlgebraicVector>(vl,pr);
}

pair<AlgebraicMatrix, AlgebraicVector> FourMomentumKinematicConstraint::derivative(const vector<RefCountedKinematicParticle> par) const
{
 int nStates = par.size();
 if (nStates == 0) 
   throw VertexException("FourMomentumKinematicConstraint::Empty vector of particles passed");
 //if (nStates != 1) throw VertexException("FourMomentumKinematicConstraint::Multi particle refit is not supported in this version");
 
 AlgebraicMatrix dr(4,7,0);
 AlgebraicVector pr(7*nStates, 0);

 for (int i = 0; i < nStates; i++)
 {
   pr.sub(i*7 + 1, asHepVector<7>(par[i]->currentState().kinematicParameters().vector()));
   dr(1,i*7 + 4) = 1.;
   dr(2,i*7 + 5) = 1.;
   dr(3,i*7 + 6) = 1.;
   if (massIsEnergy) {
     dr(4,i*7 + 7) = 1.;
   } else {
     double invenergy = 1./sqrt(pr(i*7 + 4)*pr(i*7 + 4) +
           pr(i*7 + 5)*pr(i*7 + 5) +
           pr(i*7 + 6)*pr(i*7 + 6) +
           pr(i*7 + 7)*pr(i*7 + 7));
     dr(4,i*7 + 4) = pr(i*7 + 4)*invenergy;
     dr(4,i*7 + 5) = pr(i*7 + 5)*invenergy;
     dr(4,i*7 + 6) = pr(i*7 + 6)*invenergy;
     dr(4,i*7 + 7) = pr(i*7 + 7)*invenergy;
   }
 }
 
 return pair<AlgebraicMatrix, AlgebraicVector>(dr, pr);
}

AlgebraicVector FourMomentumKinematicConstraint::deviations(int nStates) const
{
 if (nStates == 0) 
   throw VertexException("FourMomentumKinematicConstraint::Empty vector of particles passed");
 //if(nStates != 1) throw VertexException("FourMomentumKinematicConstraint::Multi particle refit is not supported in this version");

 AlgebraicVector res(7*nStates, 0);
 for (int i = 0; i < nStates; i++)
   res.sub(7*i + 1, dd);

 return res;
}

int FourMomentumKinematicConstraint::numberOfEquations() const
{
  return 4;
}

