#include "RecoVertex/ConfigurableVertexReco/interface/FitterFromLinPtFinder.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

FitterFromLinPtFinder::FitterFromLinPtFinder (
    const LinearizationPointFinder & f ) :
  theLinPtFinder ( f.clone() )
{}

FitterFromLinPtFinder::~FitterFromLinPtFinder()
{
  delete theLinPtFinder;
}

FitterFromLinPtFinder::FitterFromLinPtFinder ( const FitterFromLinPtFinder & o ) :
  theLinPtFinder ( o.theLinPtFinder->clone() )
{}

FitterFromLinPtFinder * FitterFromLinPtFinder::clone () const
{
  return new FitterFromLinPtFinder ( *this );
}

CachingVertex<5> FitterFromLinPtFinder::vertex(const vector<reco::TransientTrack> & tracks) const
{
  GlobalPoint p = theLinPtFinder->getLinearizationPoint( tracks );
  GlobalError err;
  vector<CachingVertex<5>::RefCountedVertexTrack> tks;
  return CachingVertex<5>( p, err, tks, 0. );
}
 
CachingVertex<5> FitterFromLinPtFinder::vertex( const vector<CachingVertex<5>::RefCountedVertexTrack> & tracks) const
{
  edm::LogError("") << "::vertex(VertexTracks) not implemented!";
  return CachingVertex<5>();
}

CachingVertex<5> FitterFromLinPtFinder::vertex( const vector<CachingVertex<5>::RefCountedVertexTrack> & tracks,
    const reco::BeamSpot & spot ) const
{
  return vertex ( tracks );
}


CachingVertex<5> FitterFromLinPtFinder::vertex( const vector<reco::TransientTrack> & tracks, 
    const GlobalPoint& linPoint) const
{
  return vertex ( tracks );
}

CachingVertex<5> FitterFromLinPtFinder::vertex( const vector<reco::TransientTrack> & tracks, 
    const GlobalPoint& priorPos, const GlobalError& priorError) const
{
  return vertex ( tracks );
}

CachingVertex<5> FitterFromLinPtFinder::vertex( const vector<reco::TransientTrack> & tracks, 
                      const reco::BeamSpot& beamSpot) const
{
  return vertex ( tracks );
}

CachingVertex<5> FitterFromLinPtFinder::vertex(const vector< 
    CachingVertex<5>::RefCountedVertexTrack >  & tracks, 
  const GlobalPoint& priorPos, const GlobalError& priorError) const
{
  return vertex ( tracks );
}
