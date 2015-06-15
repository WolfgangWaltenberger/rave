#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableKalmanFitter.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"

namespace {
  edm::ParameterSet mydefaults()
  {
    edm::ParameterSet ret;
    ret.addParameter<double>("maxDistance",0.01);
    ret.addParameter<int>("maxNbrOfIterations",10);
    ret.addParameter<bool>("smoothing",false);
    return ret;
  }
}

ConfigurableKalmanFitter::ConfigurableKalmanFitter() :
    AbstractConfFitter ( KalmanVertexFitter()  )
{}

void ConfigurableKalmanFitter::configure(
    const edm::ParameterSet & n )
{
  edm::ParameterSet m=n;
  m.augment ( mydefaults() );
  if (theFitter ) delete theFitter;
  bool smooth = m.getParameter<bool>("smoothing");
  theFitter = new KalmanVertexFitter( m, smooth ) ;
}

ConfigurableKalmanFitter::~ConfigurableKalmanFitter()
{
  // if (theFitter) delete theFitter;
}

ConfigurableKalmanFitter::ConfigurableKalmanFitter 
    ( const ConfigurableKalmanFitter & o ) :
  AbstractConfFitter ( o )
{}


ConfigurableKalmanFitter * ConfigurableKalmanFitter::clone() const
{
  return new ConfigurableKalmanFitter ( *this );
}

edm::ParameterSet ConfigurableKalmanFitter::defaults() const
{
  return mydefaults();
}

#include "RecoVertex/ConfigurableVertexReco/interface/ConfFitterBuilder.h"

namespace {
  ConfFitterBuilder < ConfigurableKalmanFitter > t ( "kalman", "Standard Kalman Filter" );
  ConfFitterBuilder < ConfigurableKalmanFitter > s ( "default", "Standard Kalman Filter" );
}
