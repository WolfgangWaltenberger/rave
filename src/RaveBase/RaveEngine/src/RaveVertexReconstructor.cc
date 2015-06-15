#include "RaveBase/RaveEngine/interface/RaveVertexReconstructor.h"
#include "RaveBase/RaveEngine/interface/RaveParameterSetBuilder.h"
#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#ifdef HAS_GSF
#include "TrackingTools/TransientTrack/interface/GsfTransientTrack.h"
#endif

using namespace std;

vector < TransientVertex > rave::VertexReconstructor::vertices
  ( const vector < reco::TransientTrack > & t ) const
{
  if ( t.size() == 0 )
  {
    return vector < TransientVertex > ();
  }
  // edm::LogError("rave::VertexReconstructor" ) << "t.size=" << t.size();
  // edm::LogError("rave::VertexReconstructor" ) << "t[0]=" << t[0].initialFreeState();
  #ifdef HAS_GSF
  const reco::GsfTransientTrack * gsft = dynamic_cast < const reco::GsfTransientTrack * > (t[0].basicTransientTrack() );
  if ( gsft )
  {
    // edm::LogError("rave::VertexReconstructor" ) << "gsft=" << gsft->initialFreeState();
    TrajectoryStateOnSurface tsos=gsft->stateOnSurface( GlobalPoint(0.,0.,0.) );
    vector < TrajectoryStateOnSurface > comps = tsos.components();
    /*
    edm::LogError("rave::VertexReconstructor" ) << "tsos=" << comps.size();
    for ( vector< TrajectoryStateOnSurface >::const_iterator i=comps.begin(); i!=comps.end() ; ++i )
    {
      edm::LogError("rave::VertexReconstructor") << "  `- comp=" << i->weight() << ", " << *(i->freeState());
    }
     */

  }
  #endif 
  // cout << "[RaveVertexReconstructor] w/o seed" << endl;
  return theRector->vertices ( t );
}

vector < TransientVertex > rave::VertexReconstructor::vertices
  ( const vector < reco::TransientTrack > & t, const GlobalPoint & seed ) const
{
  if ( t.size() == 0 )
  {
    return vector < TransientVertex > ();
  }
  // edm::LogError("rave::VertexReconstructor" ) << "t.size=" << t.size();
  // edm::LogError("rave::VertexReconstructor" ) << "t[0]=" << t[0].initialFreeState();
  #ifdef HAS_GSF
  const reco::GsfTransientTrack * gsft = dynamic_cast < const reco::GsfTransientTrack * > (t[0].basicTransientTrack() );
  if ( gsft )
  {
    // edm::LogError("rave::VertexReconstructor" ) << "gsft=" << gsft->initialFreeState();
    TrajectoryStateOnSurface tsos=gsft->stateOnSurface( GlobalPoint(0.,0.,0.) );
    vector < TrajectoryStateOnSurface > comps = tsos.components();
    /*
    edm::LogError("rave::VertexReconstructor" ) << "tsos=" << comps.size();
    for ( vector< TrajectoryStateOnSurface >::const_iterator i=comps.begin(); i!=comps.end() ; ++i )
    {
      edm::LogError("rave::VertexReconstructor") << "  `- comp=" << i->weight() << ", " << *(i->freeState());
    }
     */

  }
  #endif 
  // cout << "[RaveVertexReconstructor] w/ seed" << endl;
  return theRector->vertices ( t, seed );
}

vector < TransientVertex > rave::VertexReconstructor::vertices
  ( const vector < reco::TransientTrack > & t, const reco::TransientTrack & ghost ) const
{
  edm::LogError("rave::VertexReconstructor" ) << "rave doesnt have ghosts!";
  return theRector->vertices ( t );
}


vector < TransientVertex > rave::VertexReconstructor::vertices
  ( const vector < reco::TransientTrack > & t,
    const reco::BeamSpot & s ) const
{
  return theRector->vertices ( t, s );
}

vector < TransientVertex > rave::VertexReconstructor::vertices
  ( const vector < reco::TransientTrack > & t,
    const reco::BeamSpot & s,
    const reco::TransientTrack & ghost_track ) const
{
  // cout << "RaveVertexReconstructor using method vertices(t, ghosttrack, s)" << endl;
  edm::LogError("rave::VertexReconstructor" ) << "rave doesnt have ghosts!";
  return theRector->vertices( t, s );
}

vector < TransientVertex > rave::VertexReconstructor::vertices
  ( const vector < reco::TransientTrack > & prims,
    const vector < reco::TransientTrack > & secs,
    const reco::BeamSpot & s ) const
{
  return theRector->vertices ( prims, secs, s );
}

vector < TransientVertex > rave::VertexReconstructor::vertices
  ( const vector < reco::TransientTrack > & prims,
    const vector < reco::TransientTrack > & secs,
    const reco::BeamSpot & s,
    const reco::TransientTrack & ghost_track) const
{
  // cout << "RaveVertexReconstructor using method vertices(p, t, ghosttrack, s)" << endl;
  edm::LogError("rave::VertexReconstructor" ) << "rave doesnt have ghosts!";
  return theRector->vertices ( prims, secs, s );
}

rave::VertexReconstructor::~VertexReconstructor()
{
  if ( theRector ) delete theRector;
}

rave::VertexReconstructor::VertexReconstructor ( const std::string & d ) :
  theRector ( 0 ), theMethod ( d )
{
  edm::ParameterSet set = rave::ParameterSetBuilder::create ( d );
  theRector = new ConfigurableVertexReconstructor ( set );
  if (!theRector ) 
  {
    edm::LogError ( "rave::VertexReconstructor" ) 
      << "no reconstructor instantiated!";
  }
}

const string & rave::VertexReconstructor::method() const
{
  return theMethod;
}

rave::VertexReconstructor::VertexReconstructor ( const rave::VertexReconstructor & o ) :
  theRector ( 0 ), theMethod ( o.theMethod )
{
  if ( o.theRector ) theRector=o.theRector->clone();
}

rave::VertexReconstructor * rave::VertexReconstructor::clone() const
{
  return new rave::VertexReconstructor ( *this );
}
