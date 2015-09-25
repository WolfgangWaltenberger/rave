#include "RaveBase/RaveInterface/rave/VertexFactory.h"
#include "RaveBase/RaveEngine/interface/RaveBeamSpotSingleton.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "DataFormats/GeometrySurface/interface/BlockWipedAllocator.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"
#include "RaveBase/Converters/interface/CmsToRaveObjects.h"
#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include "RaveBase/RaveEngine/interface/RaveVertexReconstructor.h"
#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"
#include "RaveTools/Converters/interface/PropagatorSingleton.h"
#include "RaveBase/Converters/interface/PropagatorWrapper.h"
#include "RaveBase/Converters/interface/MagneticFieldWrapper.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>
#include <iomanip>

using namespace rave;
using namespace std;

namespace {
  rave::VertexReconstructor * getRector ( const std::string & method )
  {
    if ( method == "none" )
    {
      return (rave::VertexReconstructor *) 0;
    }
    return new rave::VertexReconstructor ( method );
  }
}

VertexFactory::VertexFactory ( const rave::MagneticField & field,
                   const rave::Propagator & prop,
                   const std::string & method,
                   int verbosity ) :
  theRector ( getRector ( method ) ), theMethod ( method ),
  theField ( field.copy() ) , theProp ( prop.copy() ),
  theVerbosity ( verbosity )
  
{
  edm::LogInfo ( "VertexFactory" ) << " factory called w/o beamspot!";
  // createRecoBeamSpot();
  setup();
}

VertexFactory::VertexFactory ( const rave::MagneticField & field,
                   const rave::Propagator & prop,
                   const rave::Ellipsoid3D & beamspot,
                   const std::string & method,
                   int verbosity ) :
  theRector ( getRector ( method ) ), theMethod ( method ),
  theField ( field.copy() ) , theProp ( prop.copy() ),
  theVerbosity ( verbosity )
{
  rave::BeamSpotSingleton::set ( beamspot );
  // cout << "[rave::VertexFactory] before setup 2" << endl;
  setup();
}

void VertexFactory::setup()
{
  MagneticFieldSingleton::Instance()->registry ( new MagneticFieldWrapper ( *theField ) );
  PropagatorSingleton::Instance()->initialise(); // init the analytical propagator
  if ( dynamic_cast < rave::VacuumPropagator * > ( theProp ) == 0 )
  {
    // not a vacuum propagator, so we register in the singleton
    PropagatorWrapper w ( *theProp );
    PropagatorSingleton::Instance()->registry ( w );

  }

  BlockWipedPoolAllocated::usePool();
}

VertexFactory::~VertexFactory()
{
    // Should we let the OS do the cleaning? Itd be more efficient.
    // cout << "[VertexFactory] destor!" << endl;
    if ( theField ) delete theField;
    MagneticFieldSingleton::Instance()->release();
    if ( theProp) delete theProp;
    PropagatorSingleton::Instance()->release();
    if ( theRector ) delete theRector;
    for ( map < string, rave::VertexReconstructor * >::const_iterator i=theRectors.begin(); 
          i!=theRectors.end() ; ++i )
    {
      delete i->second;
    }
    theRectors.clear();
    wipe(); // no need to wipe no more
}

void VertexFactory::setBeamSpot ( const rave::Ellipsoid3D & b )
{
  // cout << "VertexFactory::setBeamSpot" << endl;
  rave::BeamSpotSingleton::set ( b );
}

vector < Vertex > VertexFactory::create ( const vector < Track > & trks, const
    Point3D & seed, bool use_bs ) const
{
  if ( !theRector )
  {
    edm::LogError("rave::VertexFactory") << "trying to use uninitialised factory!";
    return vector < Vertex > ();
  }
  vector < Track > empty;
  vector < Vertex > ret= fit ( empty, trks, *theRector, seed, 
                               true, use_bs, false );
  return ret;
}


vector < Vertex > VertexFactory::create ( const vector < Track > & trks, bool use_bs ) const
{
  if ( !theRector )
  {
    edm::LogError("rave::VertexFactory") << "trying to use uninitialised factory!";
    return vector < Vertex > ();
  }
  vector < Track > empty;
  vector < Vertex > ret= fit ( empty, trks, *theRector, rave::Point3D(), 
                               false, use_bs, false );
  return ret;
}

vector < Vertex > VertexFactory::create ( const vector < Track > & trks, const Track & ghost_track, bool use_bs ) const
{   
  cout << "rave::VertexFactory using methode create with trks, ghost_track, use_bs" << use_bs << endl;
    if ( !theRector )
    {
      edm::LogError("rave::VertexFactory") << "trying to use uninitialised factory!";
      return vector < Vertex > ();
    }
    vector < Track > empty;
    vector < Vertex > ret = fit ( empty, trks, *theRector, 
        rave::Point3D(), false, use_bs, false, ghost_track ); 
    return ret;
}

vector < Vertex > VertexFactory::create ( const vector < Track > & prims,
     const vector < Track > & secs, bool use_bs ) const
{
    if ( !theRector )
    {
      edm::LogError("rave::VertexFactory") << "trying to use uninitialised factory!";
      return vector < Vertex > ();
    }
    vector < Vertex > ret = fit ( prims, secs, *theRector,
       rave::Point3D(), false, use_bs, true );
    return ret;
}

vector < Vertex > VertexFactory::create ( const vector < Track > & prims,
					  const vector < Track > & secs,
					  const Track & ghost_track, bool use_bs ) const
{
    cout << "rave::VertexFactory using methode create with prims, trks, ghost_track, use_bs" << endl;
    if ( !theRector )
    {
      edm::LogError("rave::VertexFactory") << "trying to use uninitialised factory!";
      return vector < Vertex > ();
    }
    vector < Vertex > ret = fit ( prims, secs, *theRector, 
        rave::Point3D(), false, use_bs, true, ghost_track );
    return ret;
}

vector < Vertex > VertexFactory::create ( 
    const vector < Track > & trks, const string & method, bool use_bs ) const
{
  rave::VertexReconstructor * tmp = theRectors[method];
  if ( !tmp )
  {
    tmp = getRector ( method );
    if (!tmp)
    {
      edm::LogError("rave::VertexFactory") << "cannot initialse \"" << method << "\"";
      return vector < Vertex > ();
    }
    theRectors[method] = tmp;
  }
  vector < Vertex > ret = fit ( vector < Track >(), trks, *(tmp),
     rave::Point3D(), false, use_bs, false );
  return ret;
}

vector < Vertex > VertexFactory::create ( 
    const vector < Track > & trks, const rave::Point3D & seed, 
    const string & method, bool use_bs ) const
{
  rave::VertexReconstructor * tmp = theRectors[method];
  if ( !tmp )
  {
    tmp = getRector ( method );
    if (!tmp)
    {
      edm::LogError("rave::VertexFactory") << "cannot initialse \"" << method << "\"";
      return vector < Vertex > ();
    }
    theRectors[method] = tmp;
  }
  vector < Vertex > ret = fit ( vector < Track >(), trks, *(tmp),
     seed, true, use_bs, false );
  return ret;
}


vector < Vertex > VertexFactory::create ( const vector < Track > & trks, const string & method, 
					  const Track & ghost_track, bool use_bs ) const
{
  cout << "rave::VertexFactory using methode create with trks, method, ghost_track, use_bs" << endl;
  rave::VertexReconstructor * tmp = theRectors[method];
  if ( !tmp )
  {
    tmp = getRector ( method );
    if (!tmp)
    {
      edm::LogError("rave::VertexFactory") << "cannot initialse \"" << method << "\"";
      return vector < Vertex > ();
    }
    theRectors[method] = tmp;
  }
  vector < Vertex > ret = fit ( vector < Track >(), trks, *(tmp), 
      rave::Point3D(), false, use_bs, false, ghost_track );
  return ret;
}


vector < Vertex > VertexFactory::create ( const vector < Track > & prims, const vector < Track > & secs, 
                                          const string & method, bool use_bs ) const
{
  rave::VertexReconstructor * tmp = theRectors[method];
  if ( !tmp )
  {
    tmp = getRector ( method );
    if (!tmp)
    {
      edm::LogError("rave::VertexFactory") << "cannot initialse \"" << method << "\"";
      return vector < Vertex > ();
    }
    theRectors[method] = tmp;
  }
  vector < Vertex > ret = fit ( prims, secs, *(tmp), 
      rave::Point3D(), false, use_bs, true );
  return ret;
}

void VertexFactory::wipe() const
{
  blockWipedPool().wipe();
  blockWipedPool().clear();
}

vector < Vertex > VertexFactory::create ( const vector < Track > & prims, const vector < Track > & secs, 
                                          const string & method, const Track & ghost_track, bool use_bs ) const
{
  cout << "rave::VertexFactory using methode create with prims, trks,method, ghost_track, use_bs" << endl;
  rave::VertexReconstructor * tmp = theRectors[method];
  if ( !tmp )
  {
    tmp = getRector ( method );
    if (!tmp)
    {
      edm::LogError("rave::VertexFactory") << "cannot initialse \"" << method << "\"";
      return vector < Vertex > ();
    }
    theRectors[method] = tmp;
  }
  vector < Vertex > ret = fit ( prims, secs, *(tmp), 
      rave::Point3D(), false, use_bs, true, ghost_track ); 
  return ret;
}

VertexFactory VertexFactory::operator= ( const VertexFactory & o )
{
  LogDebug("rave::VertexFactory") << "assignment operator!";
  VertexFactory ret;
  if ( o.theProp && o.theField )
  {
    ret = VertexFactory ( *(o.theField), *(o.theProp), o.theMethod, o.theVerbosity );
  }

  return ret;
}

VertexFactory::VertexFactory ( const VertexFactory & o ) :
   theRector ( 0 ), theMethod ( o.theMethod ), theField ( 0 ),
   theProp ( 0 ), theVerbosity ( o.theVerbosity )
{
  LogDebug("rave::VertexFactory") << "copy constructor.";
  if ( o.theRector ) theRector = o.theRector->clone();
  if ( o.theField ) theField = o.theField->copy();
  if ( o.theProp ) theProp = o.theProp->copy();
  for ( map < string, rave::VertexReconstructor *  >::const_iterator i=o.theRectors.begin();
        i!=theRectors.end() ; ++i )
  {
    theRectors[i->first]=i->second->clone();
  }
}

vector < Vertex > VertexFactory::fit ( const vector < Track > & prims,
   const vector < Track > & secs, const rave::VertexReconstructor & r,
   const rave::Point3D & seed, bool use_seed,
   bool use_bs, bool use_prims ) const
{
  wipe();
  try {
    vector < rave::Vertex > ret;
    vector < reco::TransientTrack > ttrks;
    vector < reco::TransientTrack > prim_trks;
    RaveToCmsObjects fconverter;
    for ( vector< Track >::const_iterator i=secs.begin(); i!=secs.end() ; ++i )
    {
        ttrks.push_back ( fconverter.tTrack ( *i ) );
    }
    if ( use_prims )
    {
      for ( vector< Track >::const_iterator i=prims.begin(); i!=prims.end() ; ++i )
      {
          prim_trks.push_back ( fconverter.tTrack ( *i ) );
      }
    }
  
    vector < TransientVertex > tmp;

    try {
      // LogDebug("") << "use_bs=" << use_bs;
      if ( use_bs && rave::BeamSpotSingleton::hasBeamSpot () )
      {
        const reco::BeamSpot * bs = rave::BeamSpotSingleton::instance();
        // LogDebug("") << "   bs=" << (void *) bs;
        if ( bs == 0 )
        {
          edm::LogError("rave::VertexFactory::fit") << "Aie! No BeamSpot!";
          tmp=r.vertices ( ttrks );
        } else {
          if ( use_prims )
          {
            if ( use_seed )
            {
              edm::LogError ( "rave::VertexFactory::fit" ) <<
                "cannot use a seed with primary tracks. discard seed.";
            }
            tmp = r.vertices ( prim_trks, ttrks, *bs );
          } else {
            if ( use_seed )
            {
              edm::LogError ( "rave::VertexFactory::fit" ) <<
                "cannot use a seed with beamspot. discard seed.";
            }
            tmp = r.vertices ( ttrks, *bs );
          }
        }
      } else { // ok, no beamspot
        if ( use_prims )
        {
          edm::LogError("") << "Aie! Trying to use primaries, but no BeamSpot!!";
        }
        if ( use_seed )
        {
          tmp = r.vertices ( ttrks, fconverter.convert(seed) );
        } else {
          tmp = r.vertices ( ttrks );
        }
      }
    } catch ( cms::Exception & e ) {
      edm::LogError("rave::VertexFactory") << "when trying to fit: " << e.what();
    } catch ( std::exception & e ) {
      edm::LogError("rave::VertexFactory") << "when trying to fit: " << e.what();
    }
    
    if ( theVerbosity>1 )
    {
      edm::LogInfo("rave::VertexFactory") 
          << "produced " << ret.size() << " vertices from " << ttrks.size() << " tracks.";
    };
    CmsToRaveObjects converter;
  
    for ( vector< TransientVertex >::const_iterator i=tmp.begin();
          i!=tmp.end() ; ++i )
    {
      vector < Track > trks = secs;
      /*
      for ( vector< Track >::const_iterator j=secs.begin(); j!=secs.end() ; ++j )
        trks.push_back ( *j );
        */
      if ( use_prims ) copy ( prims.begin(), prims.end(), back_inserter ( trks ) );
      ret.push_back ( converter.convert (*i, trks ) );
    }
  
    if ( theVerbosity>1 )
    {
        cout << "[rave::VertexFactory] produced " << ret.size() << " vertices from " << ttrks.size()
                  << " tracks." << endl;
    };
 
    return ret;
  } catch ( VertexException & e ) {
    edm::LogError("rave::VertexFactory") << "Caught VertexException saying \"" << e.what() << "\".";
    return vector < Vertex > ();
  }
}

vector < Vertex > VertexFactory::fit ( const vector < Track > & prims,
   const vector < Track > & secs, const rave::VertexReconstructor & r,
   const rave::Point3D & seed, bool use_seed,
   bool use_bs, bool use_prims, const rave::Track & ghost_track ) const
{
  wipe();
  try {
    vector < rave::Vertex > ret;
    vector < reco::TransientTrack > ttrks;
    vector < reco::TransientTrack > prim_trks;
    reco::TransientTrack ghost_trk;
    RaveToCmsObjects fconverter;
    for ( vector< Track >::const_iterator i=secs.begin(); i!=secs.end() ; ++i )
    {
        ttrks.push_back ( fconverter.tTrack ( *i ) );
    }
    if ( use_prims )
    {
      for ( vector< Track >::const_iterator i=prims.begin(); i!=prims.end() ; ++i )
      {
          prim_trks.push_back ( fconverter.tTrack ( *i ) );
      }
    }

    ghost_trk = fconverter.tTrack(ghost_track);
  
    vector < TransientVertex > tmp;
    // LogDebug("") << "use_bs=" << use_bs;
    if ( use_bs && rave::BeamSpotSingleton::hasBeamSpot () )
    {
      const reco::BeamSpot * bs = rave::BeamSpotSingleton::instance();
      // LogDebug("") << "   bs=" << (void *) bs;
      if ( bs == 0 )
      {
        edm::LogError("rave::VertexFactory::fit") << "Aie! No BeamSpot!";
        tmp=r.vertices ( ttrks );
      } else {
        if ( use_prims )
        {
          tmp = r.vertices ( prim_trks, ttrks, *bs, ghost_trk );
        } else {
          tmp = r.vertices ( ttrks, *bs, ghost_trk );
        }
      }
    } else {
      if ( use_prims )
      {
        edm::LogError("") << "Aie! Trying to use primaries, but no BeamSpot!!";
      }
      tmp = r.vertices ( ttrks, ghost_trk );
    }
  
    if ( theVerbosity>1 )
    {
      edm::LogInfo("rave::VertexFactory") 
          << "produced " << ret.size() << " vertices from " << ttrks.size() << " tracks.";
    };
    CmsToRaveObjects converter;
  
    for ( vector< TransientVertex >::const_iterator i=tmp.begin();
          i!=tmp.end() ; ++i )
    {
      vector < Track > trks = secs;
      /*
      for ( vector< Track >::const_iterator j=secs.begin(); j!=secs.end() ; ++j )
        trks.push_back ( *j );
        */
      if ( use_prims ) copy ( prims.begin(), prims.end(), back_inserter ( trks ) );
      ret.push_back ( converter.convert (*i, trks) );
    }
  
    if ( theVerbosity>1 )
    {
        cout << "[rave::VertexFactory] produced " << ret.size() << " vertices from " << ttrks.size()
                  << " tracks." << endl;
    };
 
    return ret;
  } catch ( VertexException & e ) {
    edm::LogError("rave::VertexFactory") << "Caught VertexException saying \"" << e.what() << "\".";
    return vector < Vertex > ();
  }


  //******************************************************************
  //vector < rave::Vertex > ret = fit(prims, secs, r, use_bs, use_prims);
  //vector < TransientVertex > tmp;
  //tmp = r.vertices ();
  //return ret;
}

int VertexFactory::verbosity() const
{
  return theVerbosity;
}

const std::string & VertexFactory::method() const
{
  return theMethod;
}

bool VertexFactory::hasBeamspot() const
{
  return rave::BeamSpotSingleton::hasBeamSpot();
}

const rave::Ellipsoid3D & VertexFactory::getBeamspot() const
{
  return rave::BeamSpotSingleton::get();
}

const rave::Propagator & VertexFactory::getPropagator() const
{
    return *theProp;
}

const rave::MagneticField & VertexFactory::getMagneticField() const
{
    return *theField;
}

const rave::VertexReconstructor & VertexFactory::reconstructor() const
{
  return (*theRector);
}

void VertexFactory::setDefaultMethod ( const string & method )
{
  if ( theRector ) delete theRector;
  theRector = getRector ( method );
  theMethod=method;
}

