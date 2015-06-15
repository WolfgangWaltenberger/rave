#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableSkeletonReconstructor.h"
#include "RecoVertex/SkeletonVertexReco/interface/SkeletonVertexReconstructor.h"

using namespace std;

ConfigurableSkeletonReconstructor::ConfigurableSkeletonReconstructor() :
    theRector( new SkeletonVertexReconstructor() )
{}

void ConfigurableSkeletonReconstructor::configure(
    const edm::ParameterSet & n )
{
  // whatever you need to do to configure your algorithm, 
  // do it here.
}

ConfigurableSkeletonReconstructor::~ConfigurableSkeletonReconstructor()
{
  if ( theRector ) delete theRector;
}

ConfigurableSkeletonReconstructor::ConfigurableSkeletonReconstructor 
    ( const ConfigurableSkeletonReconstructor & o ) :
  theRector ( o.theRector->clone() )
{}


ConfigurableSkeletonReconstructor * ConfigurableSkeletonReconstructor::clone() const
{
  return new ConfigurableSkeletonReconstructor ( *this );
}

vector < TransientVertex > ConfigurableSkeletonReconstructor::vertices ( 
    const std::vector < reco::TransientTrack > & t ) const
{
  return theRector->vertices ( t );
}

edm::ParameterSet ConfigurableSkeletonReconstructor::defaults() const
{
  /// this method is meant to return the default parameter settings
  return edm::ParameterSet();
}

#include "RecoVertex/ConfigurableVertexReco/interface/ConfRecoBuilder.h"

namespace {
  // this constructor registers the reconstructor in the framework
  // under the name "skeleton", with the description
  // "a skeleton implementation"
  ConfRecoBuilder < ConfigurableSkeletonReconstructor > t
    ( "skeleton", "a skeleton implementation" );
}
