#include <vector>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoBTag/SecondaryVertex/interface/SimpleSecondaryVertexComputer.h"
#include "RecoBTag/SecondaryVertex/interface/VertexFilter.h"
#include "RecoBTag/SecondaryVertex/interface/VertexSorting.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

class SimpleVertexBasedTagger {
  public:
    SimpleVertexBasedTagger ( const edm::ParameterSet & set, const reco::BeamSpot * );
    /**
     *  Perform b-tagging.
     */
    float tag (  const std::vector < reco::TransientTrack > & fitTracks,
                 const reco::Vertex & primary, const GlobalVector & jetaxis ) const;

    /**
     *  Propagate new beamspot ...
     */
    void setBeamSpot ( const reco::BeamSpot * s );
  private:
    float tag ( const std::vector < TransientVertex > & secondaries,
                const reco::Vertex & primary, const GlobalVector & jetaxis ) const;

  private:
	  bool				useBeamConstraint;
	  edm::ParameterSet		vtxRecoPSet;
	  bool	withPVError;
    reco::VertexFilter			vertexFilter;
    reco::VertexSorting			vertexSorting;
    const reco::BeamSpot * beamSpot;
    SimpleSecondaryVertexComputer theComputer;

};

