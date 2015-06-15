#ifndef _FitterFromLinPtFinder_H_
#define _FitterFromLinPtFinder_H_

#include "RecoVertex/VertexTools/interface/LinearizationPointFinder.h"
#include "RecoVertex/VertexPrimitives/interface/VertexFitter.h"

/**
 *  Wrap any LinearizationPointFinder into the VertexReconstructor interface
 */

class FitterFromLinPtFinder : public VertexFitter<5>
{
  public:
    FitterFromLinPtFinder ( const LinearizationPointFinder & );
    FitterFromLinPtFinder ( const FitterFromLinPtFinder & o );
    ~FitterFromLinPtFinder();

    CachingVertex<5> vertex ( const std::vector < reco::TransientTrack > & t ) const;
    CachingVertex<5> vertex( const std::vector< 
        CachingVertex<5>::RefCountedVertexTrack > & tracks) const;
    CachingVertex<5> vertex( const std::vector<reco::TransientTrack> & tracks, 
        const GlobalPoint& linPoint) const;
    CachingVertex<5> vertex( const std::vector<reco::TransientTrack> & tracks, 
        const GlobalPoint& priorPos, const GlobalError& priorError) const;
    CachingVertex<5> vertex( const std::vector<reco::TransientTrack> & tracks, 
                          const reco::BeamSpot& beamSpot) const;
    CachingVertex<5> vertex(const std::vector<CachingVertex<5>::RefCountedVertexTrack> & tracks, 
      const GlobalPoint& priorPos, const GlobalError& priorError) const;
    CachingVertex<5> vertex(const std::vector<CachingVertex<5>::RefCountedVertexTrack> & tracks, 
      const reco::BeamSpot & spot ) const;

    FitterFromLinPtFinder * clone () const;

  private:
    const LinearizationPointFinder * theLinPtFinder;
};

#endif
