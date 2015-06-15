#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableMultiVertexFitter.h"
#include "RecoVertex/MultiVertexFit/interface/MultiVertexReconstructor.h"
#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "RecoVertex/MultiVertexFit/interface/MultiVertexBSeeder.h"
#include "RecoVertex/VertexTools/interface/DeterministicAnnealing.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexSmoother.h"
#include "RecoVertex/VertexTools/interface/DummyVertexSmoother.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

namespace {
  edm::ParameterSet mydefaults()
  {
    edm::ParameterSet ret;
    ret.addParameter<double>("sigmacut",9.0);
    ret.addParameter<double>("Tini",8.0);
    ret.addParameter<double>("ratio",0.25);
    ret.addParameter<double>("Tfin",1.0);
    ret.addParameter<int>("cheat",0);
    ret.addParameter<bool>("smoothing",0);
    edm::ParameterSet nest;
    nest.addParameter<string>("finder","mbs");
    ret.addParameter<string>("annealing","geo");
    ret.addParameter<edm::ParameterSet>("ini",nest);
    return ret;
  }

  const VertexSmoother<5>* smoother ( const edm::ParameterSet & m )
  {
    bool s = m.getParameter<bool>("smoothing");
    if ( s ) return new KalmanVertexSmoother();
    return new DummyVertexSmoother<5>();
  }

  const AnnealingSchedule * schedule ( const edm::ParameterSet & m )
  {
    if(m.getParameter<string>("annealing") == "det")
      {
        float Tini = m.getParameter<float>("Tini"); 
        float Tfin = m.getParameter<float>("Tfin");
        float ratio = m.getParameter<float>("ratio");
        vector < float > floats;
        if (Tini > Tfin)
          {
            float Tcurr = Tini;
            if ( Tini > 0 && Tfin > 0 && ratio > 0. && ratio < 1.  )
              {
                while(Tcurr > Tfin)
                  {
                    Tcurr*=ratio;
                    floats.push_back(Tcurr);
                  }
              }
            else
              {
                LogDebug("AnnealingSchedule") << "invalid initial or final Temperature or ratio!!";
              }
          }
        else
          {
            LogDebug("AnnealingSchedule") << "using default values of deterministic annealing!!";
            floats.push_back ( 1.0 );
            floats.push_back ( 0.5 );
          }

        return new DeterministicAnnealing ( floats,
            (float) m.getParameter<double>("sigmacut") );
      }
    
    if(m.getParameter<string>("annealing") == "geo")
      {
        return new GeometricAnnealing(
        m.getParameter<double>("sigmacut"), 
        m.getParameter<double>("Tini"),
        m.getParameter<double>("ratio") );
      }
    edm::LogError("ConfigurableMultiVertexFitter")
      << "annealing schedule " << m.getParameter<string>("annealing")
      << " unknown!";
    return new GeometricAnnealing();
  }
  
  const VertexReconstructor * initialiser ( const edm::ParameterSet & p )
  {
    // cout << "[ConfigurableMultiVertexFitter] ini: " << p << endl;
    return new ConfigurableVertexReconstructor ( p );
  } 
}

ConfigurableMultiVertexFitter::ConfigurableMultiVertexFitter() :
  theRector ( new MultiVertexReconstructor( MultiVertexBSeeder() ) ),
  theCheater(0)
{}

void ConfigurableMultiVertexFitter::configure(
    const edm::ParameterSet & n )
{
  edm::ParameterSet m=n;
  edm::ParameterSet defaults=mydefaults();
  m.augment ( defaults );
  const AnnealingSchedule * ann = schedule ( m );
  const VertexSmoother<5> * smooth = smoother ( m );
  const edm::ParameterSet & nested = m.getParameter<edm::ParameterSet>("ini");
  string finder = nested.getParameter<string>("finder");
  if ( finder == "mvf" )
  {
    string def=defaults.getParameter<edm::ParameterSet>("ini").getParameter<string>("finder");
    edm::LogError("ConfigurableMultiVertexFitter") 
       << "error -- cannot be my own finder"
       << "-- fall back to " << def;
    nested.addParameter<string>("finder",def);
  }
  // cout << "[ConfigurableMultiVertexFitter] nested=" << nested << endl;
  const VertexReconstructor * ini = initialiser ( nested );
  if ( theRector ) delete theRector;
  theRector = new MultiVertexReconstructor( *ini, *ann, *smooth );
  theCheater=m.getParameter<int>("cheat");
  delete smooth;
  delete ann;
  delete ini;
}

ConfigurableMultiVertexFitter::~ConfigurableMultiVertexFitter()
{
  if ( theRector ) delete theRector;
}

ConfigurableMultiVertexFitter::ConfigurableMultiVertexFitter 
    ( const ConfigurableMultiVertexFitter & o ) :
  theRector ( o.theRector->clone() ),
  theCheater(o.theCheater)
{}


ConfigurableMultiVertexFitter * ConfigurableMultiVertexFitter::clone() const
{
  return new ConfigurableMultiVertexFitter ( *this );
}

std::vector < TransientVertex > ConfigurableMultiVertexFitter::vertices ( 
    const std::vector < reco::TransientTrack > & t,
    const reco::BeamSpot & s ) const
{
  return theRector->vertices ( t, s );
}

std::vector < TransientVertex > ConfigurableMultiVertexFitter::vertices ( 
    const std::vector < reco::TransientTrack > & t,
    const reco::BeamSpot & s,
    const reco::TransientTrack & ghost_track ) const
{
  // cout << "ConfigurableMultiVertexFitter: method vertices(t, s, ghost_track ) is called" << endl;
  return theRector->vertices ( t, s, ghost_track );
}

std::vector < TransientVertex > ConfigurableMultiVertexFitter::vertices ( 
    const std::vector < reco::TransientTrack > & prims,
    const std::vector < reco::TransientTrack > & secs,
    const reco::BeamSpot & s ) const
{
  return theRector->vertices ( prims, secs, s );
}

std::vector < TransientVertex > ConfigurableMultiVertexFitter::vertices ( 
    const std::vector < reco::TransientTrack > & prims,
    const std::vector < reco::TransientTrack > & secs,
    const reco::BeamSpot & s,
    const reco::TransientTrack & ghost_track ) const
{
  // cout << "ConfigurableMultiVertexFitter: method vertices(prims, secs, s, ghost_track) is called" << endl;
  return theRector->vertices ( prims, secs, s, ghost_track );
}

vector < TransientVertex > ConfigurableMultiVertexFitter::vertices ( 
    const std::vector < reco::TransientTrack > & t ) const
{
  return theRector->vertices ( t );
}

edm::ParameterSet ConfigurableMultiVertexFitter::defaults() const
{
  return mydefaults();
}

#include "RecoVertex/ConfigurableVertexReco/interface/ConfRecoBuilder.h"

namespace {
  ConfRecoBuilder < ConfigurableMultiVertexFitter > t ( "mvf", "Multi Vertex Fitter" );
}
