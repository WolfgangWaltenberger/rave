#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableAdaptiveGaussianSumFilter.h"
#include "RecoVertex/ConfigurableVertexReco/interface/ReconstructorFromFitter.h"
#include "RecoVertex/GaussianSumVertexFit/interface/AdaptiveGsfVertexFitter.h"
#include "RecoVertex/LinearizationPointFinders/interface/DefaultLinearizationPointFinder.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

namespace {
  edm::ParameterSet mydefaults()
  {
    edm::ParameterSet ret;
    ret.addParameter<double>("maxshift",0.01);
    ret.addParameter<double>("maxlpshift",0.1);
    ret.addParameter<int>("maxstep",100);
    ret.addParameter<bool>("limitComponents",true);
    ret.addParameter<double>("weightthreshold",0.001);
    edm::ParameterSet merger;
    merger.addParameter<int>("maxNbrComponents",4);
    merger.addParameter<string>("merger","CloseComponentsMerger");
    merger.addParameter<string>("distance","KullbackLeiblerDistance");
    ret.addParameter<edm::ParameterSet > ( "GsfMergerParameters", merger);
    // ret.addParameter<bool>("smoothTracks",true);
    return ret;
  }
}

ConfigurableAdaptiveGaussianSumFilter::ConfigurableAdaptiveGaussianSumFilter() :
    AbstractConfFitter ( AdaptiveGsfVertexFitter( mydefaults(), DefaultLinearizationPointFinder()  ) )
{}

void ConfigurableAdaptiveGaussianSumFilter::configure(
    const edm::ParameterSet & n )
{
  edm::ParameterSet m=n;
  m.augment ( mydefaults() );
  // cout << "[ConfigurableAdaptiveGaussianSumFilter] smoothTracks: " << m.getParameter<bool>("smoothTracks") << endl;
  DefaultLinearizationPointFinder linpt;
  if (theFitter) delete theFitter;
  AdaptiveGsfVertexFitter * fitter = new AdaptiveGsfVertexFitter ( m, linpt );
  theFitter=fitter;
}

ConfigurableAdaptiveGaussianSumFilter::~ConfigurableAdaptiveGaussianSumFilter()
{
  /*
  if (theFitter) delete theFitter;
  theFitter=0;
  */
}

ConfigurableAdaptiveGaussianSumFilter::ConfigurableAdaptiveGaussianSumFilter 
    ( const ConfigurableAdaptiveGaussianSumFilter & o ) :
  AbstractConfFitter ( o )
{}

ConfigurableAdaptiveGaussianSumFilter * ConfigurableAdaptiveGaussianSumFilter::clone() const
{
  return new ConfigurableAdaptiveGaussianSumFilter ( *this );
}

edm::ParameterSet ConfigurableAdaptiveGaussianSumFilter::defaults() const
{
  return mydefaults();
}

#include "RecoVertex/ConfigurableVertexReco/interface/ConfFitterBuilder.h"

namespace {
  ConfFitterBuilder < ConfigurableAdaptiveGaussianSumFilter > t ( "agsf", "AdaptiveGaussianSumFilter (not yet functional)" );
}
