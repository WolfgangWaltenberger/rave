#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableZvresReconstructor.h"

using namespace std;

ConfigurableZvresReconstructor::ConfigurableZvresReconstructor() :
    theRector( new ZvresVertexReconstructor() )
{}

void ConfigurableZvresReconstructor::configure(
    const edm::ParameterSet & n )
{
  bool debug=true;
  vector < string > params = n.getParameterNames(); 
  for ( vector< string >::const_iterator i=params.begin(); 
        i!=params.end() ; ++i )
  {
    if ((*i)=="finder") continue;
    if (debug)
      cout << "[ConfigurableZvresReconstructor] debug: " << *i << "=" << n.getParameter<double>( *i ) << endl;
    const_cast < ZvresVertexReconstructor *> (theRector)->setDoubleParameter ( *i, n.getParameter<double>( *i ) );
  }
}

ConfigurableZvresReconstructor::~ConfigurableZvresReconstructor()
{
  delete theRector;
}

ConfigurableZvresReconstructor::ConfigurableZvresReconstructor 
    ( const ConfigurableZvresReconstructor & o ) :
  theRector ( o.theRector->clone() )
{}


ConfigurableZvresReconstructor * ConfigurableZvresReconstructor::clone() const
{
  return new ConfigurableZvresReconstructor ( *this );
}

vector < TransientVertex > ConfigurableZvresReconstructor::vertices ( 
    const std::vector < reco::TransientTrack > & t ) const
{
  return theRector->vertices ( t );
}

vector < TransientVertex > ConfigurableZvresReconstructor::vertices ( 
    const std::vector < reco::TransientTrack > & t,
    const reco::BeamSpot & s ) const
{
  return theRector->vertices ( t, s );
}

edm::ParameterSet ConfigurableZvresReconstructor::defaults() const
{
  edm::ParameterSet set;
  set.addParameter<double>("Kip",1.0);
  set.addParameter<double>("Kalpha",5.0);
  set.addParameter<double>("TwoProngCut",10.0);
  set.addParameter<double>("TrackTrimCut",10.0);
  set.addParameter<double>("ResolverCut",0.6);
  return set;
}

#include "RecoVertex/ConfigurableVertexReco/interface/ConfRecoBuilder.h"

namespace {
  ConfRecoBuilder < ConfigurableZvresReconstructor > t
    ( "zvres", "Topological vertexing" );
}
