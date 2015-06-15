#include "RaveBase/RaveInterface/rave/BasicKinematicParticle.h"

#include "RaveBase/Converters/interface/CmsToRaveObjects.h"
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include "RaveBase/Converters/interface/RaveToPerigeeObjects.h"
#include "RaveBase/RaveEngine/interface/RaveId.h"

#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"

#include <iostream>

#include "boost/assert.hpp"
#include "boost/cast.hpp"

using namespace std;

namespace rave
{

const Vector7D & BasicKinematicParticle::fullstate() const
{
  BOOST_ASSERT (
    CmsToRaveObjects().convert (
      boost::any_cast< ::RefCountedKinematicParticle > ( theParticle )->currentState().kinematicParameters()
    ) ==
    theState );
  return theState;
}

const Covariance7D & BasicKinematicParticle::fullerror() const
{
  BOOST_ASSERT (
    CmsToRaveObjects().convert (
      boost::any_cast< ::RefCountedKinematicParticle > ( theParticle )->currentState().kinematicParametersError()
    ) ==
    theError );
  return theError;
}

const Vector6D & BasicKinematicParticle::state() const
{
  return fullstate();
}

const Covariance6D & BasicKinematicParticle::error() const
{
  return fullerror();
}

Charge BasicKinematicParticle::charge() const
{
  BOOST_ASSERT (
    boost::numeric_cast<Charge> (
      boost::any_cast< ::RefCountedKinematicParticle > ( theParticle )->currentState().particleCharge()
    ) ==
    theCharge );
  return theCharge;
}

const PerigeeParameters5D & BasicKinematicParticle::perigeeParameters() const
{
  return fullPerigeeParameters();
}

const PerigeeCovariance5D & BasicKinematicParticle::perigeeCovariance() const
{
  return fullPerigeeCovariance();
}

const PerigeeParameters6D & BasicKinematicParticle::fullPerigeeParameters() const
{
  if (!hasCachedPerigeeParameters) {
    cachedPerigeeParameters = RaveToPerigeeObjects().convert(fullstate(), charge() );
    hasCachedPerigeeParameters = true;
  }
  return cachedPerigeeParameters;
}

const PerigeeCovariance6D & BasicKinematicParticle::fullPerigeeCovariance() const
{
  if (!hasCachedPerigeeCovariance) {
    cachedPerigeeCovariance = RaveToPerigeeObjects().convert(fullerror(), fullstate(), charge());
    hasCachedPerigeeCovariance = true;
  }
  return cachedPerigeeCovariance;
}

/* const KinematicParticle & BasicKinematicParticle::closestToPoint(
  const rave::Point3D & point) const
{
  RaveToCmsObjects frameworkInputConverter;

  GlobalPoint ref = frameworkInputConverter.convert(point);

  // Propagate the parameters to the point closest to the reference point
  FreeTrajectoryState fts(frameworkInputConverter.convert(state(), charge()),
                          frameworkInputConverter.convert(error()));
  TrajectoryStateClosestToPoint tscp = TSCPBuilderNoMaterial()(fts, ref);

  FreeTrajectoryState propagated = tscp.theState();

  GlobalPoint  position = propagated.position();
  GlobalVector momentum = propagated.momentum();
  AlgebraicVector7 av;
  av[0] = position.x();
  av[1] = position.y();
  av[2] = position.z();
  av[3] = momentum.x();
  av[4] = momentum.y();
  av[5] = momentum.z();
  av[6] = parameters.m();
  KinematicParameters internalParams(av);

  CmsToRaveObjects frameworkOutputConverter;

  Covariance7D
      propagated_error(frameworkOutputConverter.convert(propagated.cartesianError()),
                       0., 0., 0., 0., 0., 0., covariance.dmm());

  KinematicParametersError input =
      frameworkInputConverter.convert(propagated_error);

  KinematicPerigeeConversions parameterConverter;
  ExtendedPerigeeTrajectoryError epte =
      parameterConverter.extendedPerigeeErrorFromKinematicParametersError(
      input, internalParams, charge, ref, MagneticFieldSingleton::Instance());

  // It would be cheap to use the PerigeeTrajectoryParameters twice and return the
  // PerigeeParameters6D as well, but that could let SWIG stumble.
  return frameworkOutputConverter.convert(epte);
  
  ::RefCountedKinematicParticle internal( ::VirtualKinematicParticle() );
} */

float BasicKinematicParticle::chi2() const
{
  BOOST_ASSERT (
    boost::numeric_cast<float> (
      boost::any_cast< ::RefCountedKinematicParticle > ( theParticle )->chiSquared()
    ) ==
    theChi2 );
  return theChi2;
}

float BasicKinematicParticle::ndof() const
{
  BOOST_ASSERT (
    boost::numeric_cast<float> (
      boost::any_cast< ::RefCountedKinematicParticle > ( theParticle )->degreesOfFreedom()
    ) ==
    theNdof );
  return theNdof;
}

boost::shared_ptr< MagneticField > BasicKinematicParticle::magneticField() const
{
  return theField;
}

/*
KinematicTree BasicKinematicParticle::correspondingTree() const
{
  return theTree;
}
*/

boost::shared_ptr< rave::KinematicConstraint > BasicKinematicParticle::lastConstraint() const
{
  return theConstraint;
}

int BasicKinematicParticle::id() const
{
  return theId;
}

boost::any BasicKinematicParticle::link() const
{
  return theLink;
}

std::string BasicKinematicParticle::tag() const
{
  return theTag;
}

bool BasicKinematicParticle::isValid() const
{
  return theIsValid;
}

BasicKinematicParticle * rave::BasicKinematicParticle::clone() const
{
  return new BasicKinematicParticle ( *this );
}

bool BasicKinematicParticle::operator< ( const BasicKinematicParticle & o ) const
{
  return ( id() < o.id() );
}

BasicKinematicParticle::BasicKinematicParticle ( 
    const BasicKinematicParticle & o ) : 
  theState ( o.theState ), theError ( o.theError ),
  theCharge ( o.theCharge ), 
  hasCachedPerigeeParameters ( o.hasCachedPerigeeParameters ),
  cachedPerigeeParameters ( o.cachedPerigeeParameters ),
  hasCachedPerigeeCovariance ( o.hasCachedPerigeeCovariance ),
  cachedPerigeeCovariance ( o.cachedPerigeeCovariance ),
  theNdof ( o.theNdof ), theChi2 ( o.theChi2 ), 
  theField ( o.theField ), theConstraint ( o.theConstraint ),
  theId ( o.theId ), theLink ( o.theLink ), theTag ( o.theTag ),
  theIsValid ( o.theIsValid ), theParticle ( o.theParticle )
{
    // cout << "[BasicKinematicParticle] copy constructing " << theId << endl;
}

BasicKinematicParticle::BasicKinematicParticle (
  boost::any particle, boost::any link,
  std::string tag ) :
    theState ( CmsToRaveObjects().convert ( boost::any_cast< ::RefCountedKinematicParticle > ( particle )->currentState().kinematicParameters() ) ),
    theError ( CmsToRaveObjects().convert ( boost::any_cast< ::RefCountedKinematicParticle > ( particle )->currentState().kinematicParametersError() ) ),
    theCharge ( boost::numeric_cast<Charge> ( boost::any_cast< ::RefCountedKinematicParticle > ( particle )->currentState().particleCharge() ) ),
    hasCachedPerigeeParameters( false ), hasCachedPerigeeCovariance( false ),
    theNdof ( boost::numeric_cast<float> ( boost::any_cast< ::RefCountedKinematicParticle > ( particle )->degreesOfFreedom() ) ),
    theChi2 ( boost::numeric_cast<float> ( boost::any_cast< ::RefCountedKinematicParticle > ( particle )->chiSquared() ) ),
    theId ( RaveId::uniqueId() ), theLink ( link ),
    theTag ( tag ), theIsValid ( true ),
    theParticle ( particle )
{
    // cout << "[BasicKinematicParticle]      constructing " << theId << endl;
}

BasicKinematicParticle::BasicKinematicParticle (
  int id, boost::any particle, boost::any link,
  std::string tag ) :
    theState ( CmsToRaveObjects().convert ( boost::any_cast< ::RefCountedKinematicParticle > ( particle )->currentState().kinematicParameters() ) ),
    theError ( CmsToRaveObjects().convert ( boost::any_cast< ::RefCountedKinematicParticle > ( particle )->currentState().kinematicParametersError() ) ),
    theCharge ( boost::numeric_cast<Charge> ( boost::any_cast< ::RefCountedKinematicParticle > ( particle )->currentState().particleCharge() ) ),
    hasCachedPerigeeParameters( false ), hasCachedPerigeeCovariance( false ),
    theNdof ( boost::numeric_cast<float> ( boost::any_cast< ::RefCountedKinematicParticle > ( particle )->degreesOfFreedom() ) ),
    theChi2 ( boost::numeric_cast<float> ( boost::any_cast< ::RefCountedKinematicParticle > ( particle )->chiSquared() ) ),
    theId ( id ), theLink ( link ),
    theTag ( tag ), theIsValid ( true ),
    theParticle ( particle )
{
    // cout << "[BasicKinematicParticle]     constructing " << theId << endl;
}

BasicKinematicParticle::BasicKinematicParticle () :
    hasCachedPerigeeParameters( false ), hasCachedPerigeeCovariance( false ),
    theId ( -1 ), theIsValid ( false )
{
    // cout << "[BasicKinematicParticle] default constructing " << theId << endl;
}

BasicKinematicParticle::~BasicKinematicParticle()
{
    // cout << "[BasicKinematicParticle] destroying " << theId << endl;
}

boost::any BasicKinematicParticle::getInternal() const
{
  return theParticle;
}

void BasicKinematicParticle::setInternal ( const boost::any & o )
{
  theParticle = o;
}

void BasicKinematicParticle::unlink ( )
{
  setInternal ( boost::any() );
  theLink = boost::any();
}


}
