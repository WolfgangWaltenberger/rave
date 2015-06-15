#include "RecoBTag/SecondaryVertex/interface/SimpleVertexBasedTagger.h"
#include "RecoBTag/SecondaryVertex/interface/SVBuilder.h"
#include "RecoBTag/SecondaryVertex/interface/SVFilter.h"
#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <boost/iterator/transform_iterator.hpp>

using namespace reco;
using namespace std;
using namespace edm;

SimpleVertexBasedTagger::SimpleVertexBasedTagger ( const ParameterSet & params,
   const BeamSpot * spot ) :
	useBeamConstraint(params.getParameter<bool>("useBeamConstraint")),
	vtxRecoPSet(params.getParameter<ParameterSet>("vertexReco")),
	withPVError(params.getParameter<bool>("usePVError")),
	vertexFilter(params.getParameter<ParameterSet>("vertexCuts")),
	vertexSorting(params.getParameter<ParameterSet>("vertexSelection")),
  beamSpot ( spot ),
  theComputer ( params.getParameter<ParameterSet>("simplevertexcomputer") )
{}

float SimpleVertexBasedTagger::tag (  const vector < TransientVertex > & fittedSVs,
    const Vertex & pv, const GlobalVector & jetDir ) const
{
  LogDebug("SimpleVertexBasedTagger") << "trying to tag with " << fittedSVs.size()
                                      << " TransientVertices.";
  vector<SecondaryVertex> SVs;
  SVBuilder svBuilder(pv, jetDir, withPVError);

  remove_copy_if(boost::make_transform_iterator(
                          fittedSVs.begin(), svBuilder),
                        boost::make_transform_iterator(
                          fittedSVs.end(), svBuilder),
                        back_inserter(SVs),
                        SVFilter(vertexFilter, pv, jetDir));

  LogDebug("SimpleVertexBasedTagger" ) << "after building and filtering we have " << SVs.size()
                                       << " SecondaryVertices.";

  vector<unsigned int> vtxIndices = vertexSorting(SVs);
  vector<SecondaryVertexTagInfo::VertexData> svData;
  svData.resize(vtxIndices.size());
  for(unsigned int idx = 0; idx < vtxIndices.size(); idx++) {
          const SecondaryVertex &sv = SVs[vtxIndices[idx]];

          svData[idx].vertex = sv;
          svData[idx].dist2d = sv.dist2d();
          svData[idx].dist3d = sv.dist3d();
          svData[idx].direction =
                  GlobalVector(sv.x() - pv.x(),
                               sv.y() - pv.y(),
                               sv.z() - pv.z());
  }

  SecondaryVertexTagInfo info (
				svData, SVs.size() );

  return theComputer.discriminator ( info );
}

float SimpleVertexBasedTagger::tag (  const vector < TransientTrack > & fitTracks,
    const Vertex & pv, const GlobalVector & jetDir ) const
{
  ConfigurableVertexReconstructor vertexReco(vtxRecoPSet);

  vector<TransientVertex> fittedSVs;
  if (useBeamConstraint && beamSpot )
    fittedSVs = vertexReco.vertices(fitTracks, *beamSpot);
  else
    fittedSVs = vertexReco.vertices(fitTracks);

  return tag ( fittedSVs, pv, jetDir );
}

void SimpleVertexBasedTagger::setBeamSpot ( const reco::BeamSpot * s )
{
  beamSpot=s;
}
