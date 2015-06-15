#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexPrimitives/interface/CachingVertex.h"
#include "RecoVertex/VertexPrimitives/interface/VertexState.h"
#include "RecoVertex/VertexPrimitives/interface/VertexTrack.h"


class VertexMerger : public VertexDistance3D
{

 public:

  typedef std::pair < reco::TransientTrack, float > TrackAndWeight;

  VertexMerger();
  ~VertexMerger();

  void checkDistance(const std::vector < std::pair < int, CachingVertex<5> > > newSeeds);
  CachingVertex<5> merge ( const CachingVertex<5> &, const CachingVertex<5> & ) const ;

 private:

  double mergelimit; //if reduced distance between two vertices is smaller than this constant, merge them
  /*
  int theVertexStateNr;
  mutable TrackAndSeedToWeightMap theWeights;
  */

};
