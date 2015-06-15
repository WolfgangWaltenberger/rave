#include "RecoVertex/KinematicFit/interface/EqualMassKinematicConstraint.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"


AlgebraicVector EqualMassKinematicConstraint::value (
  const vector<KinematicState> & states,
  const GlobalPoint& point ) const
{
  if ( states.size() < 2 ) throw VertexException ( "EqualMassKinematicConstraint: <2 states passed" );

  AlgebraicVector res ( 1, 0 );

  AlgebraicVector7 p1 = states[0].kinematicParameters().vector();
  AlgebraicVector7 p2 = states[1].kinematicParameters().vector();

  ParticleMass m1 = p1 ( 6 );
  ParticleMass m2 = p2 ( 6 );

  res ( 1 )  = m1 - m2;

  return res;
}

AlgebraicMatrix EqualMassKinematicConstraint::parametersDerivative ( 
    const vector<KinematicState> & states,
    const GlobalPoint& point ) const
{
  if ( states.size() < 2 ) throw VertexException ( "EqualMassKinematicConstraint: <2 states passed" );

  AlgebraicMatrix res ( 1, states.size()*7, 0 );

  //mass components: 7th and 14th elements:
  res ( 1, 7 )  = 1.0;
  res ( 1, 14 ) = -1.0;

  return res;
}

AlgebraicMatrix EqualMassKinematicConstraint::positionDerivative ( 
    const vector<KinematicState> & states,
    const GlobalPoint& point ) const
{
  if ( states.size() < 2 ) throw VertexException ( "EqualMassKinematicConstraint: <2 states passed" );

  AlgebraicMatrix res ( 1, 3, 0 );

  return res;
}

int EqualMassKinematicConstraint::numberOfEquations() const
{
  return 1;
}
