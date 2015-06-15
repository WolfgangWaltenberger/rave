#include "RaveBase/RaveInterface/rave/FlavorTagFactory.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include "RaveBase/RaveEngine/interface/RaveVertexReconstructor.h"
#include "RaveBase/Converters/interface/MagneticFieldWrapper.h"
#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"
#include "RecoBTag/SecondaryVertex/interface/SimpleVertexBasedTagger.h"
#include "RaveBase/RaveEngine/interface/RaveBeamSpotSingleton.h"
#include "RaveBase/RaveEngine/interface/RaveParameterSetBuilder.h"
#include "DataFormats/GeometrySurface/interface/BlockWipedAllocator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <iostream>
#include <iomanip>

using namespace rave;
using namespace std;

namespace {
  using namespace edm;
  GlobalVector sum ( const vector < Track > & t )
  {
    float px=0.;
    float py=0.;
    float pz=0.;
    for ( vector< Track >::const_iterator i=t.begin(); i!=t.end() ; ++i )
    {
      px+=i->state().momentum().x();
      py+=i->state().momentum().y();
      pz+=i->state().momentum().z();
    }
    return GlobalVector ( px, py, pz ) ;
  }

  inline ParameterSet myVertexRecoDefaults()
  {
    ParameterSet vertexReco;
    vertexReco.addParameter<string>("finder","avr");
    vertexReco.addParameter<bool>("smoothing",false);
    vertexReco.addParameter<float>("weightthreshold",0.001);
    vertexReco.addParameter<float>("minweight",0.5);
    vertexReco.addParameter<float>("primcut",1.8);
    vertexReco.addParameter<float>("seccut",6.0);
    vertexReco.addParameter<float>("primT",256.0);
    vertexReco.addParameter<float>("secT",256.0);
    vertexReco.addParameter<float>("primr",0.25);
    vertexReco.addParameter<float>("secr",0.25);
    return vertexReco;
  }

  inline ParameterSet myVertexCutsDefaults()
  {
    ParameterSet vertexCuts;
    vertexCuts.addParameter<bool>("useTrackWeights",true );
    vertexCuts.addParameter<float>("minimumTrackWeight",.5);
    vertexCuts.addParameter<float>("massMax",6.5);
    vertexCuts.addParameter<float>("fracPV",.65);
    vertexCuts.addParameter<int>("multiplicityMin",2);
    vertexCuts.addParameter<float>("distVal2dMin",.01);
    vertexCuts.addParameter<float>("distVal2dMax",2.5);
    vertexCuts.addParameter<float>("distVal3dMin",-99999.);
    vertexCuts.addParameter<float>("distVal3dMax", 99999.);
    vertexCuts.addParameter<float>("distSig2dMin", 3.0);
    vertexCuts.addParameter<float>("distSig2dMax", 99999.);
    vertexCuts.addParameter<float>("distSig3dMin",-99999.);
    vertexCuts.addParameter<float>("distSig3dMax", 99999.);
    vertexCuts.addParameter<float>("maxDeltaRToJetAxis",0.5);
    ParameterSet v0Filter;
    v0Filter.addParameter<float>("k0sMassWindow",0.05);
    vertexCuts.addParameter<ParameterSet>("v0Filter",v0Filter);
    return vertexCuts;
  }

  inline ParameterSet mySimpleVertexComputerDefaults()
  {
    ParameterSet simplevertexcomputer;
    simplevertexcomputer.addParameter<bool>("use3d",true);
    simplevertexcomputer.addParameter<bool>("useSignificance",true);
    simplevertexcomputer.addParameter<bool>("unBoost",false );
    return simplevertexcomputer;
  }

  inline ParameterSet myVertexSelectionDefaults()
  {
    ParameterSet vertexSelection;
    vertexSelection.addParameter<string>("sortCriterium","dist3dError");
    return vertexSelection;
  }

  inline ParameterSet mydefaults()
  {
    ParameterSet ret;
    ret.addParameter<bool>("useBeamConstraint",true);
    ret.addParameter<bool>("usePVError",true);
    ret.addParameter<ParameterSet>("vertexCuts",myVertexCutsDefaults() );
    ret.addParameter<ParameterSet>("vertexSelection",myVertexSelectionDefaults() );
    ret.addParameter<ParameterSet>("simplevertexcomputer",mySimpleVertexComputerDefaults() );

    return ret;
  }
}

FlavorTagFactory::~FlavorTagFactory()
{
  if ( theAlgo ) delete static_cast < SimpleVertexBasedTagger * > (theAlgo);
}

FlavorTagFactory::FlavorTagFactory ( const FlavorTagFactory & o ) :
   theAlgo ( 0 )
{
  edm::LogError ("FlavorTagFactory" ) << "copy constructor!";
  if ( o.theAlgo )
  {
    SimpleVertexBasedTagger * old = static_cast <  SimpleVertexBasedTagger * > (theAlgo);
    SimpleVertexBasedTagger * tmp = new SimpleVertexBasedTagger ( *old ); 
    theAlgo = (void *) (tmp);
  }
}

float FlavorTagFactory::tag ( const vector < Track > & trks,
                              const Vertex & primary, const Vector3D & axis ) const
{
  try {
    RaveToCmsObjects converter;
    GlobalVector cmsswaxis = converter.toVector ( axis );

    vector < reco::TransientTrack > ttrks;
    for ( vector< Track >::const_iterator i=trks.begin(); i!=trks.end() ; ++i )
    {
        ttrks.push_back ( converter.tTrack ( *i ) );
    }

    TransientVertex cmsswprimary = converter.convert ( primary );

    double tag = ( static_cast < SimpleVertexBasedTagger * > (theAlgo))->tag ( ttrks, cmsswprimary, cmsswaxis );
    blockWipedPool().wipe();
    blockWipedPool().clear();
    return tag;
  } catch ( cms::Exception & s ) {
    edm::LogError("FlavorTagFactory") << "caught " << s.what();
    blockWipedPool().wipe();
    blockWipedPool().clear();
    return -23.;
  }
}

FlavorTagFactory::FlavorTagFactory ( const rave::MagneticField & field,
    const string & method, const string & vtxmethod ) : theAlgo(0)
{
  MagneticFieldSingleton::Instance()->registry ( new MagneticFieldWrapper ( field ) );
  setup ( method, vtxmethod );
}

void FlavorTagFactory::setup( const string & method, const string & vtxmethod )
{
  BlockWipedPoolAllocated::usePool();
  try {
    edm::ParameterSet set = mydefaults();
    if ( vtxmethod != "" && vtxmethod != "default" )
    {
      edm::ParameterSet vtxing = rave::ParameterSetBuilder::create ( vtxmethod );
      set.addParameter<ParameterSet>("vertexReco",vtxing );
    } else {
      set.addParameter<ParameterSet>("vertexReco",myVertexRecoDefaults() );
    }

    theAlgo = (void *) (new SimpleVertexBasedTagger ( set, rave::BeamSpotSingleton::instance() ) );
  } catch ( edm::Exception & e ) {
    edm::LogError("FlavorTagFactory") << e.what();
    exit (0);
  }
}

void FlavorTagFactory::resetBeamSpot()
{
  ( static_cast < SimpleVertexBasedTagger * > (theAlgo))->setBeamSpot ( rave::BeamSpotSingleton::instance() );
}

FlavorTagFactory::FlavorTagFactory ( const rave::VertexFactory & f, const string & method )
{
  // here we know that a magnetic field exists. good.
  setup ( method, f.method() );
}

bool FlavorTagFactory::hasFlavorTagging() const
{
  #ifdef WITH_FLAVORTAGGING
  return true;
  #else
  return false;
  #endif
}

