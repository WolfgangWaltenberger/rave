#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableGaussianSumFilter.h"
#include "RecoVertex/ConfigurableVertexReco/interface/ReconstructorFromFitter.h"
#include "RecoVertex/GaussianSumVertexFit/interface/GsfVertexFitter.h"
#include "RecoVertex/LinearizationPointFinders/interface/DefaultLinearizationPointFinder.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

namespace {
  edm::ParameterSet mydefaults()
  {
    edm::ParameterSet ret;
    ret.addParameter<double>("maxDistance",0.01);
    ret.addParameter<int>("maxNbrOfIterations",10);
    ret.addParameter<bool>("limitComponents",true);
    edm::ParameterSet merger;
    merger.addParameter<int>("maxNbrComponents",4);
    merger.addParameter<string>("merger","CloseComponentsMerger");
    merger.addParameter<string>("distance","KullbackLeiblerDistance");
    ret.addParameter<edm::ParameterSet > ( "GsfMergerParameters", merger);
    ret.addParameter<bool>("smoothTracks",false);
    return ret;
  }
}

ConfigurableGaussianSumFilter::ConfigurableGaussianSumFilter() :
    AbstractConfFitter ( GsfVertexFitter( mydefaults(), DefaultLinearizationPointFinder()  ) )
{}

void ConfigurableGaussianSumFilter::configure(
    const edm::ParameterSet & n )
{
  edm::ParameterSet m=n;
  m.augment ( mydefaults() );
  DefaultLinearizationPointFinder linpt;
  if (theFitter) delete theFitter;
  GsfVertexFitter * fitter = new GsfVertexFitter ( m, linpt );
  theFitter=fitter;
}

ConfigurableGaussianSumFilter::~ConfigurableGaussianSumFilter()
{
  /*
  if (theFitter) delete theFitter;
  theFitter=0;
  */
}

ConfigurableGaussianSumFilter::ConfigurableGaussianSumFilter 
    ( const ConfigurableGaussianSumFilter & o ) :
  AbstractConfFitter ( o )
{}

ConfigurableGaussianSumFilter * ConfigurableGaussianSumFilter::clone() const
{
  return new ConfigurableGaussianSumFilter ( *this );
}

edm::ParameterSet ConfigurableGaussianSumFilter::defaults() const
{
  return mydefaults();
}

#include "RecoVertex/ConfigurableVertexReco/interface/ConfFitterBuilder.h"

namespace {
  ConfFitterBuilder < ConfigurableGaussianSumFilter > t ( "gsf", "GaussianSumFilter (not yet functional)" );
}
