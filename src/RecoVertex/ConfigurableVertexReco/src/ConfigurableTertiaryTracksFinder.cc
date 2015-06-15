#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableTertiaryTracksFinder.h"
#include "RecoVertex/TertiaryTracksVertexFinder/interface/ConfigurableTertiaryTracksVertexFinder.h"
#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "RecoVertex/AdaptiveVertexFinder/interface/AdaptiveVertexReconstructor.h"

namespace {
  edm::ParameterSet mydefaults()
  {
    edm::ParameterSet ret;
    ret.addParameter<double>("sigmacut",2.0);
    edm::ParameterSet nest;
    nest.addParameter<string>("finder","avr");
    ret.addParameter<edm::ParameterSet>("ini",nest);
    return ret;
  }

  const VertexReconstructor * initialiser ( const edm::ParameterSet & p )
  {
    // cout << "[ConfigurableTertiaryTracksFinder] ini: " << p << endl;
    return new ConfigurableVertexReconstructor ( p );
  } 
}

ConfigurableTertiaryTracksFinder::ConfigurableTertiaryTracksFinder() :
  theRector ( 0 ), thePrimaryFitter ( 0 )
{}

void ConfigurableTertiaryTracksFinder::configure(
    const edm::ParameterSet & n )
{
  edm::ParameterSet m=n;
  m.augment ( mydefaults() );
  const VertexReconstructor * ini = initialiser ( m.getParameter<edm::ParameterSet>("ini") );
  if ( theRector ) delete theRector;
  if ( thePrimaryFitter ) delete thePrimaryFitter;
  theRector = new ConfigurableTertiaryTracksVertexFinder( *ini );
  thePrimaryFitter = new AdaptiveVertexFitter ( GeometricAnnealing ( m.getParameter<double>("sigmacut") ) );
  delete ini;
}

ConfigurableTertiaryTracksFinder::~ConfigurableTertiaryTracksFinder()
{
  if ( theRector ) delete theRector;
  if ( thePrimaryFitter ) delete thePrimaryFitter;
}

ConfigurableTertiaryTracksFinder::ConfigurableTertiaryTracksFinder 
    ( const ConfigurableTertiaryTracksFinder & o ) :
  theRector ( o.theRector->clone() )
{}


ConfigurableTertiaryTracksFinder * ConfigurableTertiaryTracksFinder::clone() const
{
  return new ConfigurableTertiaryTracksFinder ( *this );
}

vector < TransientVertex > ConfigurableTertiaryTracksFinder::vertices ( 
    const std::vector < reco::TransientTrack > & t,
    const reco::BeamSpot & s ) const
{
  return vertices ( t );
}

vector < TransientVertex > ConfigurableTertiaryTracksFinder::vertices ( 
    const std::vector < reco::TransientTrack > & t ) const
{
  TransientVertex primary = thePrimaryFitter->vertex ( t );
  return theRector->vertices ( t, primary );
}

edm::ParameterSet ConfigurableTertiaryTracksFinder::defaults() const
{
  return mydefaults();
}

#include "RecoVertex/ConfigurableVertexReco/interface/ConfRecoBuilder.h"

namespace {
  ConfRecoBuilder < ConfigurableTertiaryTracksFinder > t ( "ttf", "TertiaryTracksFinder - collect tracks along the path" );
}
