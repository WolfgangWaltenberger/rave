#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableLinPtFinder.h"
#include "RecoVertex/ConfigurableVertexReco/interface/FitterFromLinPtFinder.h"
#include "RecoVertex/LinearizationPointFinders/interface/DefaultLinearizationPointFinder.h"
#include "RecoVertex/LinearizationPointFinders/interface/FsmwLinearizationPointFinder.h"

using namespace std;

namespace {
  edm::ParameterSet mydefaults()
  {
    edm::ParameterSet ret;
    ret.addParameter<int>("npairs",400 );
    ret.addParameter<float>("weightexp",-.5 );
    ret.addParameter<float>("fraction",.4 );
    ret.addParameter<float>("cut",10. );
    ret.addParameter<int>("noweightabove",5 );
    return ret;
  }
}
    
ConfigurableLinPtFinder::ConfigurableLinPtFinder() :
    AbstractConfFitter ( FitterFromLinPtFinder ( DefaultLinearizationPointFinder()  ) )
{}

void ConfigurableLinPtFinder::configure(
    const edm::ParameterSet & n )
{
  edm::ParameterSet m=n;
  m.augment ( mydefaults() );
  if ( theFitter ) delete theFitter;
  theFitter = new FitterFromLinPtFinder (// DefaultLinearizationPointFinder() );
    FsmwLinearizationPointFinder ( m.getParameter<int>("npairs"),
        m.getParameter<float>("weightexp"),
        m.getParameter<float>("fraction"),
        m.getParameter<float>("cut"),
        m.getParameter<int>("noweightabove") ) );
}

ConfigurableLinPtFinder::~ConfigurableLinPtFinder()
{
  if ( theFitter ) delete theFitter;
}

ConfigurableLinPtFinder::ConfigurableLinPtFinder 
    ( const ConfigurableLinPtFinder & o ) :
  AbstractConfFitter ( o )
{}

ConfigurableLinPtFinder * ConfigurableLinPtFinder::clone() const
{
  return new ConfigurableLinPtFinder ( *this );
}

edm::ParameterSet ConfigurableLinPtFinder::defaults() const
{
  return mydefaults();
}

#include "RecoVertex/ConfigurableVertexReco/interface/ConfFitterBuilder.h"

namespace {
  ConfFitterBuilder < ConfigurableLinPtFinder > t ( "linpt", "LinearizationPointFinder" );
}
