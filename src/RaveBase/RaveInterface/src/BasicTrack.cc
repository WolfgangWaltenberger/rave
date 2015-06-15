#include "RaveBase/RaveInterface/rave/BasicTrack.h"

#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RaveBase/Converters/interface/RaveToPerigeeObjects.h"
#include "RaveBase/RaveEngine/interface/RaveId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/PerpendicularBoundPlaneBuilder.h"
#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RaveBase/Converters/interface/CmsToRaveObjects.h"
#include "RaveBase/Converters/interface/RaveStreamers.h"
// #include "TrackingTools/GsfTools/interface/MultiTrajectoryStateCombiner.h"
#ifdef HAS_GSF
#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateAssembler.h"
#endif
#include <iostream>

using namespace rave;
using namespace std;

namespace {
  BasicTrack collapse ( const vector < pair < float, BasicTrack > > & orig )
  {
    #ifdef HAS_GSF
    if ( orig.size()==0 ) return BasicTrack();
    RaveToCmsObjects converter;
    GlobalPoint pos ( converter.convert ( orig[0].second.state().position() ) );
    GlobalVector mom ( converter.toVector ( orig[0].second.state().momentum() ) );
    const BoundPlane *bp_mom = PerpendicularBoundPlaneBuilder()( pos, mom );
    // edm::LogError ( "BasicTrack" ) << "components bound to " << pos << ", " << mom;

    vector < TrajectoryStateOnSurface > comps;
    MultiTrajectoryStateAssembler assembler;
    for ( vector< pair < float, BasicTrack > >::const_iterator i=orig.begin(); i!=orig.end() ; ++i )
    {
      FreeTrajectoryState fts = converter.convertTrackToFTS ( i->second ) ;
      TrajectoryStateOnSurface tsos_mom( fts , *bp_mom  );
      // comps.push_back (  TrajectoryStateOnSurface ( fts, *bp_mom, SurfaceSideDefinition::afterSurface, i->first ) );
      // assembler.addState (  TrajectoryStateOnSurface ( fts, *bp_mom, SurfaceSideDefinition::afterSurface, i->first ) );
      // FIXME the weight is not be supplied in the interface!!!!!
      assembler.addState (  TrajectoryStateOnSurface ( fts, *bp_mom, SurfaceSideDefinition::afterSurface ) );
    }
    // MultiTrajectoryStateCombiner combiner;
    // TrajectoryStateOnSurface collapsed = combiner.combine ( comps );
    TrajectoryStateOnSurface collapsed = assembler.combinedState();
    /*
    cout << "[BasicTrack]      collapsed is: " << collapsed.globalPosition() << ", " << collapsed.globalMomentum() << endl;
    cout << "                         error: " << collapsed.cartesianError().matrix() << endl;
     */
    CmsToRaveObjects backconversion;
    BasicTrack ret = backconversion.toBasicTrack ( collapsed );
    ret.setComponents ( orig );
    return ret;
    #endif
  }
}

BasicTrack::BasicTrack( const Vector6D & s, const Covariance6D & e, Charge q,
   float chi2, float ndof, void * p, string d )
  : theState ( s ), theError ( e ), theCharge ( q ), thePointer ( p ),
    theTag ( d ), theId ( RaveId::uniqueId() ), theIsValid ( true ),
    theChi2( chi2 ), theNdof ( ndof ),
    hasCachedPerigeeRepresentation( false )
{
  // theComponents.push_back ( *this );
  createMomPos();
}

void BasicTrack::createMomPos()
{
  theMomentum=theState.momentum();
  thePosition=theState.position();
}

BasicTrack::BasicTrack( int id, const Vector6D & s, const Covariance6D & e, Charge q,
   float chi2, float ndof, void * p, string d )
  : theState ( s ), theError ( e ), theCharge ( q ), thePointer ( p ),
    theTag ( d ), theId ( id ), theIsValid ( true ),
    theChi2( chi2 ), theNdof ( ndof ),
    hasCachedPerigeeRepresentation( false )
{
  // theComponents.push_back ( *this );
  createMomPos();
}

float BasicTrack::chi2() const
{
  return theChi2;
}

float BasicTrack::ndof() const
{
  return theNdof;
}

string BasicTrack::tag() const
{
  return theTag;
}

void BasicTrack::setComponents ( const vector < pair < float, BasicTrack > > & c )
{
  theComponents=c;
}

bool BasicTrack::isValid() const
{
  return theIsValid;
}

Charge BasicTrack::charge() const
{
  return theCharge;
}

int BasicTrack::id() const
{
  return theId;
}

const Vector6D & BasicTrack::state() const
{
  return theState;
}

const Vector3D & BasicTrack::momentum() const
{
  return theMomentum;
}

const Point3D & BasicTrack::position() const
{
  return thePosition;
}

const Covariance6D & BasicTrack::error() const
{
  return theError;
}

void * BasicTrack::originalObject() const
{
  return thePointer;
}

const PerigeeParameters5D & BasicTrack::perigeeParameters() const
{
  if (!hasCachedPerigeeRepresentation) calculateCachedPerigeeRepresentation();
  return cachedPerigeeParameters;
}

const PerigeeCovariance5D & BasicTrack::perigeeCovariance() const
{
  if (!hasCachedPerigeeRepresentation) calculateCachedPerigeeRepresentation();
  return cachedPerigeeCovariance;
}

void BasicTrack::calculateCachedPerigeeRepresentation() const
{
  RaveToPerigeeObjects converter;

  cachedPerigeeParameters = converter.convert(state(), charge());
  cachedPerigeeCovariance = converter.convert(error(), state(), charge());

  hasCachedPerigeeRepresentation = true;
}

BasicTrack * BasicTrack::clone() const
{
  return new BasicTrack ( *this );
}

BasicTrack::BasicTrack ( const vector < pair < float, BasicTrack > > & components ) :
  theIsValid ( true ), theComponents ( components )
{
  *this = collapse ( components );
}

BasicTrack::BasicTrack ( int id, const vector < pair < float, BasicTrack > > & components ) :
  theId ( id ), theIsValid ( true ), theComponents ( components )
{
  *this = collapse ( components );
  theId=id;
  createMomPos();
}


BasicTrack::BasicTrack () : theId(-1), theIsValid ( false )
{
  createMomPos();
}

vector < pair < float, BasicTrack > > BasicTrack::components() const
{
  return theComponents;
}

bool BasicTrack::operator< ( const BasicTrack & o ) const
{
    return ( id() < o.id() );
}
