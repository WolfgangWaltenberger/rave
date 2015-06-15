#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableMultiVertexBSeeder.h"
#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "RecoVertex/MultiVertexFit/interface/MultiVertexBSeeder.h"

using namespace std;

namespace {
  edm::ParameterSet mydefaults()
  {
    edm::ParameterSet ret;
    ret.addParameter<double>("nsigma", 50. );
    return ret;
  }
}

ConfigurableMultiVertexBSeeder::ConfigurableMultiVertexBSeeder() :
  theRector ( new MultiVertexBSeeder() )
{}

void ConfigurableMultiVertexBSeeder::configure(
    const edm::ParameterSet & n )
{
  edm::ParameterSet m=n;
  m.augment ( mydefaults() );
  if ( theRector ) delete theRector;
  theRector = new MultiVertexBSeeder( m.getParameter<double>("nsigma") );
}

ConfigurableMultiVertexBSeeder::~ConfigurableMultiVertexBSeeder()
{
  if ( theRector ) delete theRector;
}

ConfigurableMultiVertexBSeeder::ConfigurableMultiVertexBSeeder 
    ( const ConfigurableMultiVertexBSeeder & o ) :
  theRector ( o.theRector->clone() )
{}


ConfigurableMultiVertexBSeeder * ConfigurableMultiVertexBSeeder::clone() const
{
  return new ConfigurableMultiVertexBSeeder ( *this );
}

std::vector < TransientVertex > ConfigurableMultiVertexBSeeder::vertices ( 
    const std::vector < reco::TransientTrack > & t,
    const reco::BeamSpot & s ) const
{
  return vertices ( t );
}

std::vector < TransientVertex > ConfigurableMultiVertexBSeeder::vertices ( 
    const std::vector < reco::TransientTrack > & t,
    const reco::BeamSpot & s, 
    const reco::TransientTrack & g) const
{
  cout << "mbs: using method vertices(t, s, g)" << endl;
  return theRector->vertices ( t, s, g );
}

vector < TransientVertex > ConfigurableMultiVertexBSeeder::vertices ( 
    const std::vector < reco::TransientTrack > & t ) const
{
  return theRector->vertices ( t );
}

edm::ParameterSet ConfigurableMultiVertexBSeeder::defaults() const
{
  return mydefaults();
}

#include "RecoVertex/ConfigurableVertexReco/interface/ConfRecoBuilder.h"

namespace {
  ConfRecoBuilder < ConfigurableMultiVertexBSeeder > t ( "mbs", "Multi Vertex B-Seeder" );
}
