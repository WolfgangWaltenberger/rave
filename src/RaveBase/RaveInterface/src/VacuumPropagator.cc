#include "RaveBase/RaveInterface/rave/VacuumPropagator.h"
#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RaveBase/Converters/interface/CmsToRaveObjects.h"
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"
#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"
#include "TrackingTools/TrajectoryState/interface/PerigeeConversions.h"
#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "DataFormats/GeometrySurface/interface/GloballyPositioned.h"

#include "RaveBase/Converters/interface/RaveStreamers.h"
#include <iostream>

using namespace std;

namespace {
  // FIXME really really ugly work around for reference counting probs!
  std::vector < const TrajectoryStateOnSurface * > tsoses;
  void clearTsoses()
  {
    /*
    cout << "[VacuumPropagator] clearing!" << endl;
    for ( vector< const TrajectoryStateOnSurface * >::const_iterator 
          i=tsoses.begin(); i!=tsoses.end() ; ++i )
      delete (*i);
    tsoses.clear();
    */
  }
}
  
rave::Propagator * rave::VacuumPropagator::copy() const
{
  return new rave::VacuumPropagator ( * this );
}
    
pair < rave::Track, double > rave::VacuumPropagator::to ( const rave::Track & orig,
                          const ravesurf::Cylinder & rcyl ) const
{
  clearTsoses();
  // typedef typename GloballyPositioned<float>::PositionType PositionType;
  // typedef typename GloballyPositioned<float>::RotationType RotationType;
  typedef GloballyPositioned<float>::PositionType PositionType;
  typedef GloballyPositioned<float>::RotationType RotationType;
  RaveToCmsObjects forward;
  CmsToRaveObjects backward;
  AnalyticalPropagator prop ( MagneticFieldSingleton::Instance(),
     anyDirection );
  FreeTrajectoryState fts = forward.convertTrackToFTS ( orig );
  GlobalPoint pt = forward.convert ( rcyl.position() );
  TkRotation<float> rot;
  Cylinder cyl ( pt, rot, rcyl.radius() );
  // FIXME merde! another one of those pesky reference counting probs!
  pair < TrajectoryStateOnSurface, double > ot = prop.propagateWithPath
     ( fts, cyl );
  TrajectoryStateOnSurface * tsos = new TrajectoryStateOnSurface ( ot.first );
  tsoses.push_back ( tsos );
  rave::Track ret = backward.convert ( *tsos, orig.chi2(), orig.ndof(),
     0, orig.tag() );
  return pair < rave::Track, double > ( ret, ot.second );
}

rave::VacuumPropagator::~VacuumPropagator()
{
  clearTsoses();
}

pair < rave::Track, double > rave::VacuumPropagator::to ( const rave::Track & orig,
                          const ravesurf::Plane & rplane ) const
{
  tsoses.clear();
  rave::Track ret;
  // typedef typename GloballyPositioned<float>::PositionType PositionType;
  // typedef typename GloballyPositioned<float>::RotationType RotationType;
  typedef GloballyPositioned<float>::PositionType PositionType;
  // typedef GloballyPositioned<float>::RotationType RotationType;
  CmsToRaveObjects backward;
  RaveToCmsObjects forward;
  GlobalPoint pt = forward.convert ( rplane.position() );
  FreeTrajectoryState fts = forward.convertTrackToFTS ( orig );
  TkRotation<float> rot;
  Plane plane ( pt, rot );
  AnalyticalPropagator prop ( MagneticFieldSingleton::Instance(),
     anyDirection );
  // FIXME merde! another one of those pesky reference counting probs!
  pair < TrajectoryStateOnSurface, double > to = prop.propagateWithPath ( fts, plane );
  TrajectoryStateOnSurface * tsos = new TrajectoryStateOnSurface ( to.first );
  tsoses.push_back ( tsos );
  ret = backward.convert ( orig.id(), *tsos, orig.chi2(), orig.ndof(),
   orig.originalObject(), orig.tag() );
  return pair < rave::Track, double > ( ret, to.second );
}

rave::Track rave::VacuumPropagator::closestTo ( const rave::Track & orig,
    const rave::Point3D & pt, bool transverse ) const
{
  RaveToCmsObjects forward;
  CmsToRaveObjects backward;
  GlobalPoint pos = forward.convert ( pt );

  if ( transverse )
  {
    FreeTrajectoryState fts = forward.convertTrackToFTS ( orig );
    TransverseImpactPointExtrapolator tipe( MagneticFieldSingleton::Instance() );
    TrajectoryStateOnSurface tsos = tipe.extrapolate( fts, pos );
    rave::Track ret = backward.convert ( orig.id(), tsos, orig.chi2(), orig.ndof(),
       orig.originalObject(), orig.tag() );
    return ret;
  }
  
  PerigeeConversions conv;
  AlgebraicVector3 momentum = forward.toAlgebraicVector3 ( orig.state().momentum() );
  AlgebraicSymMatrix66 error = forward.convert ( orig.error() ).matrix();
  TrajectoryStateClosestToPoint tscp = conv.trajectoryStateClosestToPoint
    ( momentum, pos, orig.charge(), error , MagneticFieldSingleton::Instance()  );
  rave::Track ret = backward.convert ( orig.id(), tscp.theState(), orig.chi2(), 
      orig.ndof(), orig.originalObject(), orig.tag() );
  return ret;
}

rave::VacuumPropagator::VacuumPropagator() {}
