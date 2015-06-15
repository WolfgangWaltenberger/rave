#include "RaveBase/RaveInterface/rave/BasicTransientTrackKinematicParticle.h"

#include "RaveBase/Converters/interface/PerigeeToRaveObjects.h"
#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RaveBase/RaveEngine/interface/TaggedTransientTrackKinematicParticle.h"
#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicPerigeeConversions.h"
#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"

#include "boost/assert.hpp"
#include "boost/cast.hpp"

namespace rave
{

BasicTransientTrackKinematicParticle * BasicTransientTrackKinematicParticle::clone() const
{
  return new BasicTransientTrackKinematicParticle ( *this );
}

BasicTransientTrackKinematicParticle::BasicTransientTrackKinematicParticle (
  boost::any particle, boost::any origin, std::string tag ) :
    BasicKinematicParticle ( particle, origin, tag )
{
  BOOST_ASSERT ( typeid ( ::RefCountedKinematicParticle ) == particle.type() );
  BOOST_ASSERT (
    dynamic_cast< ::TransientTrackKinematicParticle * > (
      boost::any_cast< ::RefCountedKinematicParticle > ( particle ).get() ) );
}

BasicTransientTrackKinematicParticle::BasicTransientTrackKinematicParticle (
  int id, boost::any particle, boost::any origin, std::string tag ) :
    BasicKinematicParticle ( id, particle, origin, tag )
{
  BOOST_ASSERT ( typeid ( ::RefCountedKinematicParticle ) == particle.type() );
}

namespace
{
boost::shared_ptr< ::KinematicStatePropagator > propagator ( new ::TrackKinematicStatePropagator() );
::TransientTrackKinematicStateBuilder builder;

::RefCountedKinematicParticle createInternalParticle (
  const rave::Track & initialTrack, const float & massGuess,
  const float & massSigma, const float & chiSquared,
  const float & degreesOfFr )
{
  // Convert rave::Track to reco::TransientTrack
  boost::shared_ptr< reco::TransientTrack > recoTrack (
    new reco::TransientTrack ( RaveToCmsObjects().tTrack ( initialTrack ) ) );

  ::KinematicState initState = builder (
                                 *recoTrack,
                                 boost::numeric_cast< ::ParticleMass > ( massGuess ),
                                 boost::numeric_cast< float > ( massSigma ) );

  float chi2 = boost::numeric_cast< float > ( chiSquared );
  float ndof = boost::numeric_cast< float > ( degreesOfFr );
  ::KinematicConstraint * lastConstraint = 0;
  ReferenceCountingPointer< ::KinematicParticle > previousParticle = 0;

  ::RefCountedKinematicParticle
  taggedParticle ( new ::TaggedTransientTrackKinematicParticle (
                     initState,
                     chi2,
                     ndof,
                     lastConstraint,
                     previousParticle,
                     propagator.get(),
                     recoTrack ) );

  return taggedParticle;
}

::RefCountedKinematicParticle createInternalParticle (
  const rave::Track & initialTrack, const float & massGuess,
  const float & massSigma )
{
  return createInternalParticle ( initialTrack, massGuess, massSigma,
                                  initialTrack.chi2(), initialTrack.ndof() );
}

::RefCountedKinematicParticle createInternalParticle (
  const rave::Vector7D & state, const rave::Covariance7D & error,
  const rave::Charge & charge, const float & chiSquared,
  const float & degreesOfFr )
{
  ::KinematicParameters internalState = RaveToCmsObjects().convert ( state );
  ::KinematicParametersError internalError = RaveToCmsObjects().convert ( error );
  ::KinematicState initState = builder ( internalState, internalError,
                                         boost::numeric_cast< int > ( charge ),
                                         MagneticFieldSingleton::Instance() );

  float chi2 = boost::numeric_cast< float > ( chiSquared );
  float ndof = boost::numeric_cast< float > ( degreesOfFr );
  ::KinematicConstraint * lastConstraint = 0;
  ReferenceCountingPointer< ::KinematicParticle > previousParticle = 0;

  ::RefCountedKinematicParticle
  taggedParticle ( new ::TaggedTransientTrackKinematicParticle (
                     initState,
                     chi2,
                     ndof,
                     lastConstraint,
                     previousParticle,
                     propagator.get() ) );

  return taggedParticle;
}

::RefCountedKinematicParticle createInternalParticle (
  const rave::Vector7D & state, const rave::PerigeeCovariance6D & error,
  const rave::Charge & charge, const float & chiSquared,
  const float & degreesOfFr, const rave::Point3D & referencePoint )
{
  // Convert the perigee error to a cartesian error
  rave::Covariance7D cartesianError = PerigeeToRaveObjects().convert(error, state, charge, referencePoint);
  return createInternalParticle(state, cartesianError, charge, chiSquared, degreesOfFr);
}

::RefCountedKinematicParticle createInternalParticle (
  const rave::PerigeeParameters6D & state,
  const rave::PerigeeCovariance6D & error,
  const rave::Charge & charge, const float & chiSquared, 
  const float & degreesOfFr, const rave::Point3D & referencePoint )
{
  // Convert the perigee parameters to cartesian parameters
  rave::Vector7D cartesianParameters =
      PerigeeToRaveObjects().convert(state, charge, referencePoint);
  // Convert the perigee error to a cartesian error
  rave::Covariance7D cartesianError =
      PerigeeToRaveObjects().convert(error, cartesianParameters, charge, referencePoint);

  return createInternalParticle(cartesianParameters, cartesianError, charge,
                                chiSquared, degreesOfFr);
}

}

BasicTransientTrackKinematicParticle::BasicTransientTrackKinematicParticle (
  const rave::Track & initialTrack, const float & massGuess,
  const float & massSigma, const float & chiSquared,
  const float & degreesOfFr, boost::any origin, std::string tag ) :
    BasicKinematicParticle (
      createInternalParticle ( initialTrack, massGuess, massSigma, chiSquared,
                               degreesOfFr ),
      origin, tag )
{}

BasicTransientTrackKinematicParticle::BasicTransientTrackKinematicParticle (
  const rave::Track & initialTrack, const float & massGuess,
  const float & massSigma, boost::any origin, std::string tag ) :
    BasicKinematicParticle (
      createInternalParticle ( initialTrack, massGuess, massSigma ),
      origin, tag )
{}

BasicTransientTrackKinematicParticle::BasicTransientTrackKinematicParticle (
  const rave::Vector7D & state, const rave::Covariance7D & error,
  const rave::Charge & charge, const float & chiSquared,
  const float & degreesOfFr, boost::any origin, std::string tag ) :
    BasicKinematicParticle (
      createInternalParticle ( state, error, charge, chiSquared,
                               degreesOfFr ),
      origin, tag )
{}

BasicTransientTrackKinematicParticle::BasicTransientTrackKinematicParticle (
  const rave::Vector7D & state, const rave::PerigeeCovariance6D & error,
  const rave::Charge & charge, const double & chiSquared,
  const double & degreesOfFr, const rave::Point3D & referencePoint,
  boost::any origin, std::string tag ) :
    BasicKinematicParticle (
      createInternalParticle ( state, error, charge, chiSquared,
                               degreesOfFr, referencePoint ),
      origin, tag )
{}

BasicTransientTrackKinematicParticle::BasicTransientTrackKinematicParticle (
  const rave::PerigeeParameters6D & state,
  const rave::PerigeeCovariance6D & error, const rave::Charge & charge,
  const double & chiSquared, const double & degreesOfFr,
  const rave::Point3D & referencePoint,boost::any origin,
  std::string tag ) :
    BasicKinematicParticle (
      createInternalParticle ( state, error, charge, chiSquared, degreesOfFr,
                               referencePoint ),
      origin, tag )
{}

BasicTransientTrackKinematicParticle::BasicTransientTrackKinematicParticle() :
    BasicKinematicParticle()
{}

BasicTransientTrackKinematicParticle::~BasicTransientTrackKinematicParticle()
{}

TransientTrackKinematicParticle
BasicTransientTrackKinematicParticle::closestToTransversePoint(
    const double & x, const double & y) const
{
  RaveToCmsObjects frameworkInputConverter;

  GlobalPoint ref(x, y, 0.);

  ::FreeTrajectoryState fts(frameworkInputConverter.convert(state(), charge()),
                            frameworkInputConverter.convert(error()));
  ParticleMass mass = boost::numeric_cast< ParticleMass > ( fullstate().m() );
  float m_sigma = boost::numeric_cast< float > ( sqrt(fullerror().dmm()) );

  ::KinematicState initState = builder(fts, mass, m_sigma, ref);

  float chiSq = boost::numeric_cast< float > ( chi2() );
  float ndf = boost::numeric_cast< float > ( ndof() );
  ::KinematicConstraint * lastConstraint = 0;
  ReferenceCountingPointer< ::KinematicParticle > previousParticle = 0;

  ::RefCountedKinematicParticle
      taggedParticle ( new ::TaggedTransientTrackKinematicParticle (
                       initState,
                       chiSq,
                       ndf,
                       lastConstraint,
                       previousParticle,
                       propagator.get() ) );
  return TransientTrackKinematicParticle(
           BasicTransientTrackKinematicParticle(taggedParticle));
}

}
