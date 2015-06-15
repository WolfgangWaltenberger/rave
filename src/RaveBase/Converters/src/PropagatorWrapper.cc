#include "RaveBase/Converters/interface/PropagatorWrapper.h"
#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"
#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RaveBase/Converters/interface/CmsToRaveObjects.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include <iostream>

using namespace std;

PropagatorWrapper::PropagatorWrapper ( const rave::Propagator & r ) :
  theRavePropagator ( r.copy() )
{}

const MagneticField * PropagatorWrapper::magneticField() const
{
  return MagneticFieldSingleton::Instance();
}

pair < TrajectoryStateOnSurface, double > 
   PropagatorWrapper::propagateWithPath ( const FreeTrajectoryState & fts,
    const Plane & surf ) const
{
  RaveToCmsObjects rcconv;
  CmsToRaveObjects crconv;
  ravesurf::Plane rsurf = crconv.convert ( surf );
  // cout << "[PropagatorWrapper] ok now we propagate to plane here" << endl;
  rave::Track oldt = crconv.convert ( fts );
  // cout << "[PropagatorWrapper] track is " << oldt.id() << endl;
  pair < rave::Track, double > t = theRavePropagator->to ( oldt, rsurf );
  FreeTrajectoryState nfts = rcconv.convertTrackToFTS ( t.first );
  TrajectoryStateOnSurface st ( nfts, surf );
  st.freeState()->setTrackId ( fts.trackId() );
  return pair < TrajectoryStateOnSurface, double > ( st, t.second );
}

pair < TrajectoryStateOnSurface, double > 
    PropagatorWrapper::propagateWithPath ( const FreeTrajectoryState & fts,
    const Cylinder & surf ) const
{
  RaveToCmsObjects rcconv;
  CmsToRaveObjects crconv;
  ravesurf::Cylinder rsurf = crconv.convert ( surf );
  rave::Track oldt = crconv.convert ( fts );
  pair < rave::Track, double > t = theRavePropagator->to ( oldt, rsurf );
  FreeTrajectoryState nfts = rcconv.convertTrackToFTS ( t.first );
  TrajectoryStateOnSurface st ( nfts, surf );
  st.freeState()->setTrackId ( fts.trackId() );
  return pair < TrajectoryStateOnSurface, double > ( st, t.second );
}

PropagatorWrapper::~PropagatorWrapper()
{
  delete theRavePropagator;
}

PropagatorWrapper * PropagatorWrapper::clone() const
{
  return new PropagatorWrapper ( *this );
}

PropagatorWrapper::PropagatorWrapper ( const PropagatorWrapper & o ) :
  theRavePropagator ( o.theRavePropagator->copy() )
{}

TrajectoryStateOnSurface PropagatorWrapper::propagate ( 
    const FreeTrajectoryState & fts, const Cylinder & c ) const
{
  return propagateWithPath ( fts, c ).first;
}

TrajectoryStateOnSurface PropagatorWrapper::propagate ( 
    const FreeTrajectoryState & fts, const Plane & c ) const
{
  return propagateWithPath ( fts, c ).first;
}
