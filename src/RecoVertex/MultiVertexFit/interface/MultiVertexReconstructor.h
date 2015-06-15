#ifndef _MultiVertexReconstructor_H_
#define _MultiVertexReconstructor_H_

#include "RecoVertex/VertexPrimitives/interface/VertexReconstructor.h"
#include "RecoVertex/MultiVertexFit/interface/MultiVertexFitter.h"
#include "RecoVertex/VertexTools/interface/GeometricAnnealing.h"

/**
 *  Class that wraps the MultiVertexFitter, together with
 *  a user-supplied VertexReconstructor into a VertexReconstructor.
 */
class MultiVertexReconstructor : public VertexReconstructor
{
public:
  MultiVertexReconstructor ( const VertexReconstructor &,
                             const AnnealingSchedule & s = 
                                   GeometricAnnealing(9.,8.0,0.25),
                             const VertexSmoother<5> & v = 
                                   DummyVertexSmoother<5>(),
                             float revive=-1. );
  MultiVertexReconstructor ( const MultiVertexReconstructor & );
  ~MultiVertexReconstructor();

  std::vector<TransientVertex> vertices(const std::vector<reco::TransientTrack> &,
      const reco::BeamSpot & ) const; 
  std::vector<TransientVertex> vertices(const std::vector<reco::TransientTrack> &,
      const reco::BeamSpot &,
      const reco::TransientTrack & ghost_track ) const; 
  std::vector<TransientVertex> vertices(const std::vector<reco::TransientTrack> &) const; 
  std::vector<TransientVertex> vertices(const std::vector<reco::TransientTrack> &,
      const std::vector < reco::TransientTrack > & primaries ) const;

  std::vector<TransientVertex> vertices(const std::vector<reco::TransientTrack> &,
      const std::vector < reco::TransientTrack > & primaries,
      const reco::BeamSpot & spot ) const;

  std::vector<TransientVertex> vertices(const std::vector<reco::TransientTrack> &,
      const std::vector < reco::TransientTrack > & primaries,
      const reco::BeamSpot & spot, 
      const reco::TransientTrack & ghost_track ) const;

  VertexReconstructor * reconstructor() const;

  MultiVertexReconstructor * clone() const;

private:
  std::vector<reco::TransientTrack> collecttracks(const std::vector<reco::TransientTrack> &,
      const std::vector < reco::TransientTrack > & primaries, bool gotprimaries ) const;
  std::vector<TransientVertex> fit(const std::vector<reco::TransientTrack> &,
      const std::vector < reco::TransientTrack > & primaries,
					const reco::BeamSpot & spot, bool gotprimaries, bool usespot ) const;
   

  VertexReconstructor * theSeedingReconstructor;
  MultiVertexFitter * theFitter;
};

#endif
