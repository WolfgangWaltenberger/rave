#include "RaveBase/RaveInterface/rave/TransientTrackKinematicParticle.h"

#include "RaveBase/RaveEngine/interface/TaggedTransientTrackKinematicParticle.h"

#include "boost/cast.hpp"
#include <iostream>

using namespace std;

namespace rave
{

TransientTrackKinematicParticle::TransientTrackKinematicParticle (
  const rave::Track & initialTrack, const float & massGuess,
  const float & massSigma, const float & chiSquared,
  const float & degreesOfFr, boost::any origin, std::string tag ) :
    KinematicParticle ( BasicTransientTrackKinematicParticle (
                          initialTrack, massGuess, massSigma, chiSquared,
                          degreesOfFr, origin, tag ) )
{
  // OK, this is dirty, but a the clever ReferenceCountingPointer does not
  // allow anything else (as the shared_ptr would), here it is:
  static_cast< ::TaggedTransientTrackKinematicParticle* > (
    boost::any_cast< ::RefCountedKinematicParticle > (
      data().getInternal() ).get() )->setTag ( *this );
}

TransientTrackKinematicParticle::TransientTrackKinematicParticle (
  const rave::Track & initialTrack, const float & massGuess,
  const float & massSigma, boost::any origin, std::string tag ) :
    KinematicParticle ( BasicTransientTrackKinematicParticle (
                          initialTrack, massGuess, massSigma, origin, tag ) )
{
  /*cout << "[TransientTrackKinematicParticle] constructing " << id() 
       << " from rave track " << initialTrack.id() << endl; */
  // OK, this is dirty, but a the clever ReferenceCountingPointer does not
  // allow anything else (as the shared_ptr would), here it is:
  static_cast< ::TaggedTransientTrackKinematicParticle* > (
    boost::any_cast< ::RefCountedKinematicParticle > (
      data().getInternal() ).get() )->setTag ( *this );
}

rave::TransientTrackKinematicParticle::TransientTrackKinematicParticle (
  const rave::Vector7D & state, const rave::Covariance7D & error,
  const rave::Charge & charge, const float & chiSquared,
  const float & degreesOfFr, boost::any origin, std::string tag ) :
    KinematicParticle ( BasicTransientTrackKinematicParticle (
                          state, error, charge, chiSquared, degreesOfFr,
                          origin, tag ) )
{
  // OK, this is dirty, but a the clever ReferenceCountingPointer does not
  // allow anything else (as the shared_ptr would), here it is:
  static_cast< ::TaggedTransientTrackKinematicParticle* > (
    boost::any_cast< ::RefCountedKinematicParticle > (
      data().getInternal() ).get() )->setTag ( *this );
  /* cout << "[TransientTrackKinematicParticle] constructing from vector 7d"
       << endl; */
}

rave::TransientTrackKinematicParticle::TransientTrackKinematicParticle (
  const rave::Vector7D & state, const rave::PerigeeCovariance6D & error,
  const rave::Charge & charge, const double & chiSquared,
  const double & degreesOfFr, const rave::Point3D & referencePoint, 
  boost::any origin, std::string tag ) :
    KinematicParticle ( BasicTransientTrackKinematicParticle (
                          state, error, charge, chiSquared, degreesOfFr,
                          referencePoint, origin, tag ) )
{
  // OK, this is dirty, but a the clever ReferenceCountingPointer does not
  // allow anything else (as the shared_ptr would), here it is:
  static_cast< ::TaggedTransientTrackKinematicParticle* > (
    boost::any_cast< ::RefCountedKinematicParticle > (
      data().getInternal() ).get() )->setTag ( *this );
  /* cout << "[TransientTrackKinematicParticle] constructing from vector 7d"
       << endl; */
}

rave::TransientTrackKinematicParticle::TransientTrackKinematicParticle ( 
  const rave::PerigeeParameters6D & state,
  const rave::PerigeeCovariance6D & error,
  const rave::Charge & charge,const double & chiSquared, 
  const double & degreesOfFr, const rave::Point3D & referencePoint,
  boost::any origin, std::string tag ) :
    KinematicParticle ( BasicTransientTrackKinematicParticle (
                        state, error, charge, chiSquared, degreesOfFr, 
                        referencePoint, origin, tag ) )
{
  // OK, this is dirty, but a the clever ReferenceCountingPointer does not
  // allow anything else (as the shared_ptr would), here it is:
  static_cast< ::TaggedTransientTrackKinematicParticle* > (
      boost::any_cast< ::RefCountedKinematicParticle > (
      data().getInternal() ).get() )->setTag ( *this );
  /* cout << "[TransientTrackKinematicParticle] constructing from perigee"
       << endl; */
}


TransientTrackKinematicParticle::TransientTrackKinematicParticle (
  const BasicTransientTrackKinematicParticle & o ) :
    // KinematicParticle ( o )
    KinematicParticle ( * ( o.clone() ) )
{
  /* cout << "[TransientTrackKinematicParticle] constructing from BTTKP" 
       << endl; */

}

TransientTrackKinematicParticle::TransientTrackKinematicParticle() :
    KinematicParticle ( BasicTransientTrackKinematicParticle () )
{
  /* cout << "[TransientTrackKinematicParticle] constructing default " 
       << endl; */
}

TransientTrackKinematicParticle
    TransientTrackKinematicParticle::closestToTransversePoint(
    const double & x, const double & y) const
{
  const BasicTransientTrackKinematicParticle* bttkp = 
      dynamic_cast< const BasicTransientTrackKinematicParticle* >( &data() );
  return bttkp->closestToTransversePoint(x,y);
}

}
