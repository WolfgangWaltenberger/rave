#ifndef _MultiVertexFitter_H_
#define _MultiVertexFitter_H_

#include <vector>
#include <set>
#include <utility>
#include <map>
#include "RecoVertex/VertexPrimitives/interface/CachingVertex.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/LinearizationPointFinders/interface/DefaultLinearizationPointFinder.h"
#include "RecoVertex/MultiVertexFit/interface/LinTrackCache.h"
#include "RecoVertex/VertexTools/interface/GeometricAnnealing.h"
#include "RecoVertex/VertexTools/interface/DummyVertexSmoother.h"

class MultiVertexFitter
{
public:
  /**
   *  \class MultiVertexFitter
   *  fits n vertices in parallel, associating weights for every
   *  track-vertex std::pair. The special constructor's arguments
   *  take precedence over the SimpleConfigurables.
   *
   *  SimpleConfigurables:
   *
   *  MultiVertexFitter:Debug = 0
   *  MultiVertexFitter:DisplacementLimit = 0.00001
   *  MultiVertexFitter:MaxIterations = 30
   *  MultiVertexFitter:ClaimLostVertices = true
   *  MultiVertexFitter:MinimumWeightFraction = 1e-6
   *  MultiVertexFitter:ReviveBelow = 0.3
   *  MultiVertexFitter:DiscardLightWeights = true
   *
   */

  MultiVertexFitter( const AnnealingSchedule & sched =
                              GeometricAnnealing(9.,8.0,0.25),
                     const LinearizationPointFinder & seeder = 
                        DefaultLinearizationPointFinder(),
                     const VertexSmoother<5> & smoother = 
                        DummyVertexSmoother<5>(),
                     float revive_below = -1. );
  MultiVertexFitter( const MultiVertexFitter & );
  ~MultiVertexFitter();

  typedef std::pair < reco::TransientTrack, float > TrackAndWeight;
  typedef std::map < int , double > SeedToWeightMap;
  typedef std::map < reco::TransientTrack, SeedToWeightMap > TrackAndSeedToWeightMap;

  /**
   * Supply simple clusters of reco::TransientTracks.
   * \paramname primaries: supply tracks which are hard coded
   * to the primary vertex.
   */
  std::vector < CachingVertex<5> > vertices ( 
      const std::vector < std::vector < reco::TransientTrack > > &,
      const std::vector < reco::TransientTrack > & primaries =
       std::vector < reco::TransientTrack > () );

  /**
   *  Supply clusters of tracks with weights, association weights of the other
   *  vertices is considered zero.
   *  FIXME weights are currently ignored.
   * \paramname primaries: (optionally) supply tracks which are hard coded
   * to the primary vertex. I.e. they will not be considered for the secondary vertex.
   * \paramname beamspot: (optionally) supply the beamspot constraint.
   * Used only if usespot=true.
   */
  std::vector < CachingVertex<5> > vertices (
      const std::vector < std::vector < TrackAndWeight > > &,
      const std::vector < reco::TransientTrack > & primaries =
      std::vector < reco::TransientTrack > (),
      const reco::BeamSpot & spot = reco::BeamSpot(),
      bool usespot = false );

  /**
   * Supply further clusters of reco::TransientTracks.
   * \paramname ghost_track: userdefined or calculated jetaxis
   * used as additional information
   */
  std::vector < CachingVertex<5> > vertices(
      const std::vector < std::vector < TrackAndWeight > > &, const reco::TransientTrack & ghost_track,
      const std::vector < reco::TransientTrack > & primaries =
      std::vector < reco::TransientTrack > (),
      const reco::BeamSpot & spot = reco::BeamSpot(),
      bool usespot = false);

  /**
   *  Supply full CachingVertices; CachingVertices are the first seeds.
   * \paramname primaries: supply tracks which are hard coded
   * to the primary vertex.
   */
  std::vector < CachingVertex<5> > vertices (
      const std::vector < CachingVertex<5> > & seeds,
      const std::vector < reco::TransientTrack > & primaries =
       std::vector < reco::TransientTrack > () );

  /**
   *  Same as above.
   */
  std::vector < CachingVertex<5> > vertices ( const std::vector < TransientVertex > & seeds,
     const std::vector < reco::TransientTrack > & primaries =
     std::vector < reco::TransientTrack > () );

private:
  std::vector < CachingVertex<5> > fit( const reco::BeamSpot & spot, bool usespot );
  void lostVertexClaimer();
  void updateWeights();
  bool updateSeeds( const reco::BeamSpot & spot, bool usespot );

  void clear();
  void createSeed ( const std::vector < reco::TransientTrack > & tracks, const reco::TransientTrack & ghost_track, const reco::BeamSpot & spot,
                    bool usespot );
  void createSeed ( const std::vector < TrackAndWeight > & tracks, const reco::TransientTrack & ghost_track, const reco::BeamSpot & spot,
                    bool usespot );

  void createSeed ( const std::vector < reco::TransientTrack > & tracks, const reco::BeamSpot & spot,
                    bool usespot );
  void createSeed ( const std::vector < TrackAndWeight > & tracks, const reco::BeamSpot & spot,
                    bool usespot );

  void createPrimaries ( const std::vector < reco::TransientTrack > tracks );
  void printWeights() const;
  void printWeights( const reco::TransientTrack & ) const;
  void printSeeds() const;

  int seedNr();
  void resetSeedNr();

private:

  typedef CachingVertex<5>::RefCountedVertexTrack RefCountedVertexTrack;
  typedef ReferenceCountingPointer<LinearizedTrackState<5> > RefCountedLinearizedTrackState;

  // vertex seeds. Come with a seed number
  // makes the weight table simpler, faster,
  // and more reliable (I hope)
  std::vector < std::pair < int, CachingVertex<5> > > theVertexSeeds;
  int theVertexStateNr;
  float theReviveBelow;
  std::vector < reco::TransientTrack > theTracks;
  std::set < reco::TransientTrack > thePrimaries;
  AnnealingSchedule * theAssComp;
  LinearizationPointFinder * theSeeder;
  VertexSmoother<5> * theSmoother;
  mutable TrackAndSeedToWeightMap theWeights;
  LinTrackCache theCache;
};

#endif
