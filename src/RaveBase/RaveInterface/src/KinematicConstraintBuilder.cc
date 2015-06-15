#include "RaveBase/RaveInterface/rave/KinematicConstraintBuilder.h"

#include "RaveBase/Converters/interface/RaveToCmsObjects.h"

#include "RecoVertex/KinematicFit/interface/BackToBackKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/EqualMassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/FourMomentumKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/MomentumKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/PointingKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/SimplePointingConstraint.h"
#include "RecoVertex/KinematicFit/interface/SmartPointingConstraint.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/VertexKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/TwoTrackMassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/MultiTrackMassKinematicConstraint.h"
#include "RecoVertex/KinematicFitPrimitives/interface/MultipleKinematicConstraint.h"

#include "boost/cast.hpp"

namespace rave
{

KinematicConstraintBuilder::KinematicConstraintBuilder()
{}

KinematicConstraintBuilder::~KinematicConstraintBuilder()
{}

rave::KinematicConstraint KinematicConstraintBuilder::createMassKinematicConstraint ( const float & mass, const float sigma ) const
{
  boost::shared_ptr< ::KinematicConstraint > internalConstraint (
    new ::MassKinematicConstraint (
      boost::numeric_cast< ::ParticleMass > ( mass ),
      boost::numeric_cast< float > ( sigma ) ) );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}

rave::MultipleKinematicConstraint KinematicConstraintBuilder::createMultipleKinematicConstraint() const
{
  boost::shared_ptr< ::MultipleKinematicConstraint > internalConstraint (
    new ::MultipleKinematicConstraint () );
  return rave::MultipleKinematicConstraint ( BasicMultipleKinematicConstraint ( internalConstraint ) );
}


rave::KinematicConstraint KinematicConstraintBuilder::createBackToBackKinematicConstraint() const
{
  boost::shared_ptr< ::KinematicConstraint > internalConstraint (
    new ::BackToBackKinematicConstraint () );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}


rave::KinematicConstraint KinematicConstraintBuilder::createEqualMassKinematicConstraint() const
{
  boost::shared_ptr< ::MultiTrackKinematicConstraint > internalConstraint (
      new ::EqualMassKinematicConstraint () );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}


rave::KinematicConstraint KinematicConstraintBuilder::createFourMomentumKinematicConstraint (
  const rave::Vector4D & momentum, const rave::Vector4D & deviation,
  const bool mass_is_energy) const
{
  boost::shared_ptr< ::KinematicConstraint > internalConstraint (
    new ::FourMomentumKinematicConstraint (
      RaveToCmsObjects().toAlgebraicVector ( momentum ),
      RaveToCmsObjects().toAlgebraicVector ( deviation ), mass_is_energy ) );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}

rave::KinematicConstraint KinematicConstraintBuilder::createMomentumKinematicConstraint (
  const rave::Vector3D & momentum, const rave::Vector3D & deviation ) const
{
  boost::shared_ptr< ::KinematicConstraint > internalConstraint (
    new ::MomentumKinematicConstraint (
      RaveToCmsObjects().toAlgebraicVector ( momentum ),
      RaveToCmsObjects().toAlgebraicVector ( deviation ) ) );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}

rave::KinematicConstraint KinematicConstraintBuilder::createPointingKinematicConstraint (
  const rave::Point3D & reference ) const
{
  boost::shared_ptr< ::KinematicConstraint > internalConstraint (
    new ::PointingKinematicConstraint (
      RaveToCmsObjects().convert ( reference ) ) );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}

rave::KinematicConstraint KinematicConstraintBuilder::createSimplePointingConstraint (
  const rave::Point3D & reference ) const
{
  boost::shared_ptr< ::KinematicConstraint > internalConstraint (
    new ::SimplePointingConstraint (
      RaveToCmsObjects().convert ( reference ) ) );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}

rave::KinematicConstraint KinematicConstraintBuilder::createSmartPointingConstraint (
  const rave::Point3D & reference ) const
{
  boost::shared_ptr< ::KinematicConstraint > internalConstraint (
    new ::SmartPointingConstraint (
      RaveToCmsObjects().convert ( reference ) ) );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}

rave::KinematicConstraint KinematicConstraintBuilder::createVertexKinematicConstraint() const
{
  boost::shared_ptr< ::MultiTrackKinematicConstraint > internalConstraint (
    new ::VertexKinematicConstraint() );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}

rave::KinematicConstraint KinematicConstraintBuilder::createTwoTrackMassKinematicConstraint (
  const float & mass ) const
{
  ::ParticleMass lmass = boost::numeric_cast< ::ParticleMass > ( mass );
  boost::shared_ptr< ::MultiTrackKinematicConstraint > internalConstraint (
    new ::TwoTrackMassKinematicConstraint ( lmass ) );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}

rave::KinematicConstraint KinematicConstraintBuilder::createMultiTrackMassKinematicConstraint (
  const float & mass, int nparticles ) const
{
  ::ParticleMass lmass = boost::numeric_cast< ::ParticleMass > ( mass );
  boost::shared_ptr< ::MultiTrackKinematicConstraint > internalConstraint (
    new ::MultiTrackMassKinematicConstraint ( lmass, nparticles ) );
  return rave::KinematicConstraint ( BasicKinematicConstraint ( internalConstraint ) );
}

}
