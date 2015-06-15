#include "RecoVertex/MultiVertexFit/interface/MultiVertexFitter.h"
// #include "Vertex/VertexPrimitives/interface/TransientVertex.h"
#include <map>
#include <algorithm>
#include <iomanip>
// #include "Vertex/VertexRecoAnalysis/interface/RecTrackNamer.h"
// #include "Vertex/MultiVertexFit/interface/TransientVertexNamer.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/LinearizedTrackStateFactory.h"
#include "RecoVertex/VertexTools/interface/VertexTrackFactory.h"
#include "RecoVertex/VertexPrimitives/interface/VertexState.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexTrackCompatibilityEstimator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
// #include "RecoVertex/MultiVertexFit/interface/VertexMerger.h"

// #define MVFHarvestingDebug
#ifdef MVFHarvestingDebug
#include "Vertex/VertexSimpleVis/interface/PrimitivesHarvester.h"
#endif

using namespace std;
using namespace reco;

namespace
{
  typedef MultiVertexFitter::TrackAndWeight TrackAndWeight;
  typedef MultiVertexFitter::TrackAndSeedToWeightMap TrackAndSeedToWeightMap;
  typedef MultiVertexFitter::SeedToWeightMap SeedToWeightMap;
  typedef CachingVertex<5>::RefCountedVertexTrack RefCountedVertexTrack;

  int verbose()
  {
    static const int ret = 1; /*SimpleConfigurable<int>
      (0, "MultiVertexFitter:Debug").value(); */
    return ret;
  }

  double minWeightFraction()
  {
    // minimum weight that a track has to have
    // in order to be taken into account for the
    // vertex fit.
    // Given as a fraction of the total weight.
    static const float ret = 1e-6; /* SimpleConfigurable<float>
      (1e-6, "MultiVertexFitter:MinimumWeightFraction").value(); */
    return ret;
  }

  bool discardLightWeights()
  {
    static const bool ret = true; /* SimpleConfigurable<bool>
      (true, "MultiVertexFitter:DiscardLightWeights").value();*/
    return ret;
  }

  /*
  struct CompareRaveTracks
  {
    bool operator() ( const TransientTrack & r1,
                      const TransientTrack & r2 ) const
    {
      return r1 < r2;
    };
  }*/

  CachingVertex<5> createSeedFromLinPt ( const GlobalPoint & gp )
  {
    return CachingVertex<5> ( gp, GlobalError (),
                           vector<RefCountedVertexTrack> (), 0.0 );
  }

  double validWeight ( double weight )
  {
    if ( weight > 1.0 )
    {
      LogDebug("::validWeight") << "weight=" << weight << "??" ;
      return 1.0;
    };

    if ( weight < 0.0 )
    {
      LogDebug("::validWeight") << "weight=" << weight << "??" ;
      return 0.0;
    };
    return weight;
  }
}

void MultiVertexFitter::clear()
{
  theAssComp->resetAnnealing();
  theTracks.clear();
  thePrimaries.clear();
  theVertexSeeds.clear();
  theWeights.clear();
  theCache.clear();
}

// creates the seed, additionally it pushes all tracks
// onto theTracks
void MultiVertexFitter::createSeed( const vector < TransientTrack > & tracks, 
    const reco::BeamSpot & spot, bool usespot )
{
  try {
    if ( tracks.size() > 1 )
    {
      CachingVertex<5> vtx = createSeedFromLinPt (
         theSeeder->getLinearizationPoint ( tracks ) );//FIXME beamspotinfo hinein; wichtig: if spot !=0 muss rein
      int snr= seedNr();
      LogDebug("::createSeed") << "seedNr in createSeed1 is: " << snr ;
      theVertexSeeds.push_back ( pair < int, CachingVertex<5> > ( snr, vtx ) );
      for ( vector< TransientTrack >::const_iterator track=tracks.begin();
            track!=tracks.end() ; ++track )
      {
        theWeights[*track][snr]=1.;
        theTracks.push_back ( *track );
      };
    };
  } catch ( VertexException & e ) {
    edm::LogError("MultiVertexFitter") << "yo. really, the seed";
  }
}

void MultiVertexFitter::createSeed( const vector < TransientTrack > & tracks, const reco::TransientTrack & ghost_track, const reco::BeamSpot & spot,
                                    bool usespot )
{
  return(createSeed(tracks, spot, usespot));
}

void MultiVertexFitter::createPrimaries ( const std::vector < reco::TransientTrack > tracks )
{
  for ( vector< reco::TransientTrack >::const_iterator i=tracks.begin(); 
        i!=tracks.end() ; ++i )
  {
    thePrimaries.insert ( *i );
  }
}

int MultiVertexFitter::seedNr()
{
  return theVertexStateNr++;
}

void MultiVertexFitter::resetSeedNr()
{
  theVertexStateNr=0;
}

void MultiVertexFitter::createSeed( const vector < TrackAndWeight > & tracks, const reco::BeamSpot & spot,
    bool usespot )
{
  // create initial seed for every bundle
  vector < RefCountedVertexTrack> newTracks;//vertex track anschauen = im prinzip ein track der zu einem Vertex gehoert

  for ( vector< TrackAndWeight >::const_iterator track=tracks.begin();
        track!=tracks.end() ; ++track )
  {
    double weight = validWeight ( track->second );
    const GlobalPoint & pos = track->first.impactPointState().globalPosition();
    GlobalError err; // FIXME
    VertexState realseed ( pos, err );

    RefCountedLinearizedTrackState lTrData = 
      theCache.linTrack ( pos, track->first );

    VertexTrackFactory<5> vTrackFactory;
    RefCountedVertexTrack vTrData = vTrackFactory.vertexTrack(
      lTrData, realseed, weight );
    newTracks.push_back ( vTrData );
  };

  if ( newTracks.size() > 1 )
  {
    CachingVertex<5> vtx;
    int snr = seedNr();
    LogDebug("::createSeed") << "seedNr in createSeed2 is: " << snr ;

    if ((snr !=0)){
      LogDebug("::createSeed") << "use ghost!" ;
      //newTracks.push_back ();
    }

    if ( (snr == 0) && usespot ){
      LogDebug("::createSeed") << "use spot!" ;
      vtx = KalmanVertexFitter().vertex ( newTracks, spot );
    } else {
      LogDebug("::createSeed") << "dont use spot!" ;
      vtx = KalmanVertexFitter().vertex ( newTracks );
    }

    if (vtx.isValid())
      theVertexSeeds.push_back ( pair < int, CachingVertex<5> > ( snr, vtx ) );

    // We initialise the weights with the original
    // user supplied weights.
    for ( vector< TrackAndWeight >::const_iterator track=tracks.begin();
          track!=tracks.end() ; ++track )
    {
      if ( thePrimaries.count ( track->first ) )
      {
        theWeights[track->first][theVertexSeeds[0].first]=track->second;//wird genommen und in the Weights hinengeschrieben
        continue;
      };
      float weight = track->second;
      weight = validWeight(weight);
      theWeights[track->first][snr]=weight;
      theTracks.push_back ( track->first );
    };
  };

  // this thing will actually have to discard tracks
  // that have been submitted - attached to a different vertex - already.
  // sort ( theTracks.begin(), theTracks.end(), CompareRaveTracks() );
  sort ( theTracks.begin(), theTracks.end() );
  for ( vector< TransientTrack >::iterator i=theTracks.begin(); 
        i<theTracks.end() ; ++i )
  {
    if ( i != theTracks.begin() )
    {
      if ( (*i) == ( *(i-1) ) )
      {
        theTracks.erase ( i );
      };
    };
  };
}

void MultiVertexFitter::createSeed( const vector < TrackAndWeight > & tracks, const reco::TransientTrack & ghost_track, const reco::BeamSpot & spot,
    bool usespot )
{
  // create initial seed for every bundle
  vector < RefCountedVertexTrack> newTracks;//vertex track anschauen = im prinzip ein track der zu einem Vertex gehoert
  TrackAndWeight weighted_ghost(ghost_track, 1.0);

  for ( vector< TrackAndWeight >::const_iterator track=tracks.begin();
        track!=tracks.end() ; ++track )
  {
    double weight = validWeight ( track->second );
    const GlobalPoint & pos = track->first.impactPointState().globalPosition();
    GlobalError err; // FIXME
    VertexState realseed ( pos, err );

    RefCountedLinearizedTrackState lTrData = 
      theCache.linTrack ( pos, track->first );

    VertexTrackFactory<5> vTrackFactory;
    RefCountedVertexTrack vTrData = vTrackFactory.vertexTrack(
      lTrData, realseed, weight );
    newTracks.push_back ( vTrData );
  };

  if ( newTracks.size() > 1 )
  {
    CachingVertex<5> vtx;
    int snr = seedNr();
    LogDebug("::createSeed") << "seedNr in createSeed2 is: " << snr ;

    if ((snr !=0)){
      LogDebug("::createSeed") << "use ghost!" ;
      //newTracks.push_back ();
    }

    if ( (snr == 0) && usespot ){
      LogDebug("::createSeed") << "use spot!" ;
      vtx = KalmanVertexFitter().vertex ( newTracks, spot );
    } else {
      LogDebug("::createSeed") << "dont use spot!" ;
      vtx = KalmanVertexFitter().vertex ( newTracks );
    }

    theVertexSeeds.push_back ( pair < int, CachingVertex<5> > ( snr, vtx ) );

    // We initialise the weights with the original
    // user supplied weights.
    for ( vector< TrackAndWeight >::const_iterator track=tracks.begin();
          track!=tracks.end() ; ++track )
    {
      if ( thePrimaries.count ( track->first ) )
      {
        theWeights[track->first][theVertexSeeds[0].first]=track->second;//wird genommen und in the Weights hinengeschrieben
        continue;
      };
      float weight = track->second;
      weight = validWeight(weight);
      theWeights[track->first][snr]=weight;
      theTracks.push_back ( track->first );
    };
  };

  // this thing will actually have to discard tracks
  // that have been submitted - attached to a different vertex - already.
  // sort ( theTracks.begin(), theTracks.end(), CompareRaveTracks() );
  sort ( theTracks.begin(), theTracks.end() );
  for ( vector< TransientTrack >::iterator i=theTracks.begin(); 
        i<theTracks.end() ; ++i )
  {
    if ( i != theTracks.begin() )
    {
      if ( (*i) == ( *(i-1) ) )
      {
        theTracks.erase ( i );
      };
    };
  };



//return createSeed(tracks, spot, usespot);
}

vector < CachingVertex<5> > MultiVertexFitter::vertices (
    const vector < TransientVertex > & vtces,
    const vector < TransientTrack > & primaries )
{
  // FIXME if vtces.size < 1 return sth that includes the primaries
  if ( vtces.size() < 1 )
  {
    return vector < CachingVertex<5> > ();
  };
  vector < vector < TrackAndWeight > > bundles;
  for ( vector< TransientVertex >::const_iterator vtx=vtces.begin();
        vtx!=vtces.end() ; ++vtx )
  {
    vector < TransientTrack > trks = vtx->originalTracks();
    vector < TrackAndWeight > tnws;
    for ( vector< TransientTrack >::const_iterator trk=trks.begin(); 
          trk!=trks.end() ; ++trk )
    {
      float w = vtx->trackWeight ( *trk ); 
      if ( w > 1e-5 )
      {
        TrackAndWeight tmp ( *trk, w );
        tnws.push_back ( tmp );
      };
    };
    bundles.push_back ( tnws );
  };
  return vertices ( bundles, primaries );
}

vector < CachingVertex<5> > MultiVertexFitter::vertices (
    const vector < CachingVertex<5> > & seeds,
    const vector < TransientTrack > & primaries )
{
  createPrimaries ( primaries );
  // FIXME if seeds size < 1 return sth that includes the primaries
  if (  seeds.size() < 1 ) return seeds;
  for ( vector< CachingVertex<5> >::const_iterator vtx=seeds.begin();
        vtx!=seeds.end() ; ++vtx )
  {
    int snr = seedNr();
    theVertexSeeds.push_back ( pair < int, CachingVertex<5> >
        ( snr, *vtx ) );
    TransientVertex rvtx = *vtx;
    const vector < TransientTrack > & trks = rvtx.originalTracks();
    for ( vector< TransientTrack >::const_iterator trk=trks.begin();
          trk!=trks.end() ; ++trk )
    {
      if ( !(thePrimaries.count (*trk )) )
      {
        theTracks.push_back ( *trk );
      } else {
      }
      LogDebug("::vertices") << "error! track weight currently set to one" << " FIXME!!!" ;
      theWeights[*trk][snr]=1.0;
    };
  };
  #ifdef MVFHarvestingDebug
  for ( vector< CachingVertex<5> >::const_iterator i=theVertexSeeds.begin();
        i!=theVertexSeeds.end() ; ++i )
    PrimitivesHarvester::file()->save(*i);
  #endif
  return fit( reco::BeamSpot(), false );
}

vector < CachingVertex<5> > MultiVertexFitter::vertices (
    const vector < vector < TransientTrack > > & tracks,
    const vector < TransientTrack > & primaries )
{
  createPrimaries ( primaries );

  for ( vector< vector < TransientTrack > >::const_iterator cluster=
        tracks.begin(); cluster!=tracks.end() ; ++cluster )
  {
    createSeed ( *cluster, reco::BeamSpot(), false /* dont use spot */ );
  };
  if ( verbose() )
  {
    printSeeds();
  };
  #ifdef MVFHarvestingDebug
  for ( vector< CachingVertex<5> >::const_iterator i=theVertexSeeds.begin();
        i!=theVertexSeeds.end() ; ++i )
    PrimitivesHarvester::file()->save(*i);
  #endif
  return fit( reco::BeamSpot(), false );
}

vector < CachingVertex<5> > MultiVertexFitter::vertices (
    const vector < vector < TrackAndWeight > > & ttracks,
    const vector < TransientTrack > & primaries,
    const reco::BeamSpot & spot, 
    bool usespot )
{
  try {
    clear();
    vector < vector < TrackAndWeight > > tracks = ttracks;
    createPrimaries ( primaries );

    for ( vector< vector < TrackAndWeight > >::const_iterator cluster=
          tracks.begin(); cluster!=tracks.end() ; ++cluster )
    {
      createSeed ( *cluster, spot, usespot );
    };
    if ( verbose() )
    {
      printSeeds();
    };

    return fit( spot, usespot );
  } catch ( VertexException & e ) {
    edm::LogError ( "MultiVertexFitter" ) << "couldnt find vertices";
    return vector < CachingVertex<5> >();
  }
}

vector < CachingVertex<5> > MultiVertexFitter::vertices (
    const vector < vector < TrackAndWeight > > & tracks,
    const reco::TransientTrack & ghost_track,
    const vector < TransientTrack > & primaries,
    const reco::BeamSpot & spot, bool usespot )
{
  LogDebug("::vertices") << "Using method with ghost_track!";
  clear();
  createPrimaries ( primaries );
  TrackAndWeight weighted_ghost(ghost_track, 1.);
  //tracks.push_back(weighted_ghost);
  //createSeed (weighted_ghost, spot, usespot);

  for ( vector< vector < TrackAndWeight > >::const_iterator cluster=
        tracks.begin(); cluster!=tracks.end() ; ++cluster )
  {
    createSeed ( *cluster, ghost_track, spot, usespot );
  };
  if ( verbose() )
  {
    printSeeds();
  };

  return fit( spot, usespot );
}

MultiVertexFitter::MultiVertexFitter( const AnnealingSchedule & ann,
                                      const LinearizationPointFinder & seeder,
                                      const VertexSmoother<5> & smoother,
                                      float revive_below ) :
    theVertexStateNr ( 0 ), theReviveBelow ( revive_below ),
    theAssComp ( ann.clone() ), theSeeder ( seeder.clone() ),
    theSmoother ( smoother.clone() )
{}

MultiVertexFitter::MultiVertexFitter( const MultiVertexFitter & o ) :
    theVertexStateNr ( o.theVertexStateNr ), theReviveBelow ( o.theReviveBelow ),
    theAssComp ( o.theAssComp->clone() ), theSeeder ( o.theSeeder->clone() ),
    theSmoother ( o.theSmoother->clone() )
{}

MultiVertexFitter::~MultiVertexFitter()
{
  delete theAssComp;
  delete theSeeder;
  delete theSmoother;
}

void MultiVertexFitter::updateWeights()
{
  theWeights.clear();
  if ( verbose() & 4 )
  {
    LogDebug("::updateWeights") << "Start weight update." ;
  };

  KalmanVertexTrackCompatibilityEstimator<5> theComp;
  
  /** 
   *  add the primary only tracks to primary vertex only.
   */
  for ( set < TransientTrack >::const_iterator trk=thePrimaries.begin();
        trk!=thePrimaries.end() ; ++trk )
  {
    int seednr = theVertexSeeds[0].first;
    CachingVertex<5> seed = theVertexSeeds[0].second;
    LogDebug("::updateWeights") << "seeder is:" ;
    pair < bool, double > chi2 = theComp.estimate ( seed, theCache.linTrack ( seed.position(), *trk ) );     
    double weight = theAssComp->phi ( chi2.second );
    theWeights[*trk][seednr]= weight; // FIXME maybe "hard" 1.0 or "soft" weight?
  }

  /**
   *  now add "free tracks" to all vertices
   */
  for ( vector< TransientTrack >::const_iterator trk=theTracks.begin();
        trk!=theTracks.end() ; ++trk )
  {
    double tot_weight=theAssComp->phi ( theAssComp->cutoff() * theAssComp->cutoff() );

    for ( vector < pair < int, CachingVertex<5> > >::const_iterator 
          seed=theVertexSeeds.begin(); seed!=theVertexSeeds.end(); ++seed )
    {
      
      pair < bool, double > chi2 = theComp.estimate ( seed->second, theCache.linTrack ( seed->second.position(),
           *trk ) );
      double weight = theAssComp->phi ( chi2.second );
      tot_weight+=weight;
      theWeights[*trk][seed->first]=weight;
    };

    // normalize to sum of all weights of one track equals 1.
    // (if we include the "cutoff", as well)
    if ( tot_weight > 0.0 )
    {
      for ( vector < pair < int, CachingVertex<5> > >::const_iterator 
            seed=theVertexSeeds.begin();
            seed!=theVertexSeeds.end(); ++seed )
      {
        double normedweight=theWeights[*trk][seed->first]/tot_weight;
        if ( normedweight > 1.0 )
        {
          LogDebug("::updateWeights") << "[MultiVertexFitter] he? w[" // << TransientTrackNamer().name(*trk)
               << "," << seed->second.position() << "] = " << normedweight
               << " totw=" << tot_weight ;
          normedweight=1.0;
        };
        if ( normedweight < 0.0 )
        {
          LogDebug("::updateWeights") << "[MultiVertexFitter] he? weight=" << normedweight
               << " totw=" << tot_weight ;
          normedweight=0.0;
        };
        theWeights[*trk][seed->first]=normedweight;
      };
    } else {
      // total weight equals zero? restart, with uniform distribution!
      LogDebug("::updateWeights") << "[MultiVertexFitter] track found with no assignment - ";
      LogDebug("::updateWeights") << "will assign uniformly." ;
      float w = .5 / (float) theVertexSeeds.size();
      for ( vector < pair < int, CachingVertex<5> > >::const_iterator seed=theVertexSeeds.begin();
            seed!=theVertexSeeds.end(); ++seed )
      {
        theWeights[*trk][seed->first]=w;
      };
    };
  };
  if ( verbose() & 2 ) printWeights();
}

bool MultiVertexFitter::updateSeeds( const reco::BeamSpot & spot, bool usespot )
{
  resetSeedNr();
  double max_disp=0.;
  // need to fit with the right weights.
  // also trigger an updateWeights.
  // if the seeds dont move much we return true

  vector < pair < int, CachingVertex<5> > > newSeeds;

  for ( vector< pair < int, CachingVertex<5> > >::const_iterator seed=theVertexSeeds.begin();
        seed!=theVertexSeeds.end() ; ++seed )
  {
    // for each seed get the tracks with the right weights.
    // TransientVertex rv = seed->second;
    // const GlobalPoint & seedpos = seed->second.position();
    int snr = seed->first;
    LogDebug("::updateSeeds") << "snr in update seeds with declaration seed->first is: " << snr ;
    VertexState realseed ( seed->second.position(), seed->second.error() );
    GlobalPoint position = seed->second.position();
    LogDebug("::updateSeeds") << "position of vertex is: " << position ;

    double totweight=0.;
    for ( vector< TransientTrack >::const_iterator track=theTracks.begin();
          track!=theTracks.end() ; ++track )
    {
      totweight+=theWeights[*track][snr];
    };


    int nr_good_trks=0; // how many tracks above weight limit
    // we count those tracks, because that way
    // we can discard lightweights if there are enough tracks
    // and not discard the lightweights if that would give us
    // fewer than two tracks ( we would loose a seed, then ).
    if ( discardLightWeights() )
    {
      for ( vector< TransientTrack >::const_iterator track=theTracks.begin();
            track!=theTracks.end() ; ++track )
      {
        if ( theWeights[*track][snr] > totweight * minWeightFraction() )
        {
          nr_good_trks++;
        };
      };
    };

    vector<RefCountedVertexTrack> newTracks;
    for ( vector< TransientTrack >::const_iterator track=theTracks.begin();
          track!=theTracks.end() ; ++track )
    {
      double weight = validWeight ( theWeights[*track][snr] );
      // Now we add a track, if
      // a. we consider all tracks or
      // b. we discard the lightweights but the track's weight is high enough or
      // c. we discard the lightweights but there arent enough good tracks,
      //    so we add all lightweights again (too expensive to figure out
      //    which lightweights are the most important)
      if ( !discardLightWeights() || weight > minWeightFraction() * totweight
           || nr_good_trks < 2 )
      {
        // if the linearization point didnt move too much,
        // we take the old LinTrackState.
        // Otherwise we relinearize.

        RefCountedLinearizedTrackState lTrData =
          theCache.linTrack ( seed->second.position(), *track );

        VertexTrackFactory<5> vTrackFactory;
        RefCountedVertexTrack vTrData = vTrackFactory.vertexTrack(
          lTrData, realseed, weight );
        newTracks.push_back ( vTrData );
      };
    };

    for ( set< TransientTrack >::const_iterator track=thePrimaries.begin();
          track!=thePrimaries.end() ; ++track )
    {
      double weight = validWeight ( theWeights[*track][snr] );

      RefCountedLinearizedTrackState lTrData =
        theCache.linTrack ( seed->second.position(), *track );

      VertexTrackFactory<5> vTrackFactory;
      RefCountedVertexTrack vTrData = vTrackFactory.vertexTrack(
        lTrData, realseed, weight );
      newTracks.push_back ( vTrData );

    };

    try {
      if ( newTracks.size() < 2 )
      {
        throw VertexException("less than two tracks in vector" );
      };

      if ( verbose() )
      {
        LogDebug("::updateSeeds") << "[MultiVertexFitter] now fitting with Kalman: ";
        for ( vector< RefCountedVertexTrack >::const_iterator i=newTracks.begin(); 
              i!=newTracks.end() ; ++i )
        {
          LogDebug("::updateSeeds") << (**i).weight() << " ";
        };
        LogDebug("::updateSeeds") ;
      };



      //possible other location for vertexMerger





      if ( newTracks.size() > 1 )
      {
        KalmanVertexFitter fitter;
        // warning! first track determines lin pt!
              CachingVertex<5> newVertex;
        int snr = seedNr();
        if( snr == 0 && usespot )
        {
          LogDebug("::updateSeeds") << "Use beamspot!" ;
          newVertex = fitter.vertex ( newTracks, spot );
        } else {
          LogDebug("::updateSeeds") << "Dont use beamspot!" ;
          newVertex = fitter.vertex ( newTracks );
        }

        double disp = ( newVertex.position() - seed->second.position() ).mag();
        if ( disp > max_disp ) max_disp = disp;
        newSeeds.push_back ( pair < int, CachingVertex<5> > ( snr, newVertex ) );
      };
    } catch ( exception & e )
    {
      LogDebug("::updateSeeds") << "[MultiVertexFitter] exception: " << e.what() ;
    } catch ( ... )
    {
      LogDebug("::updateSeeds") << "[MultiVertexFitter] unknown exception." ;
    };
  };



  
  //execute VertexMerger here: merge any seeds close enough before further processing is done




  // now discard all old seeds and weights, compute new ones.
  theVertexSeeds.clear();
  theWeights.clear();
  theVertexSeeds=newSeeds;
  #ifdef MVFHarvestingDebug
  for ( vector< CachingVertex<5> >::const_iterator i=theVertexSeeds.begin();
        i!=theVertexSeeds.end() ; ++i )
    PrimitivesHarvester::file()->save(*i);
  #endif
  updateWeights();

  static const double disp_limit = 1e-4; /* SimpleConfigurable<double>
    (0.0001, "MultiVertexFitter:DisplacementLimit" ).value(); */

  if ( verbose() & 2 )
  {
    printSeeds();
    LogDebug("::updateSeeds") << "[MultiVertexFitter] max displacement in this iteration: "
         << max_disp ;
  };
  
  if ( max_disp < disp_limit ) return false;
  return true;
}

// iterating over the fits
vector < CachingVertex<5> > MultiVertexFitter::fit( const reco::BeamSpot & spot, bool usespot )
{
  try {
    LogDebug ("::fit") << "::fit usespot: " << usespot;
    if ( verbose() & 2 ) printWeights();
    int ctr=1;
    static const int ctr_max = 50; /* SimpleConfigurable<int>(100,
        "MultiVertexFitter:MaxIterations").value(); */
    while ( updateSeeds( spot, usespot ) || !(theAssComp->isAnnealed()) )
    {
      if ( ++ctr >= ctr_max ) break;
      theAssComp->anneal();
      // lostVertexClaimer(); // was a silly(?) idea to "revive" vertex candidates.
      resetSeedNr();
    };

    if ( verbose() )
    {
      LogDebug("::fit") << "[MultiVertexFitter] number of iterations: " << ctr ;
      LogDebug("::fit") << "[MultiVertexFitter] remaining seeds: "
           << theVertexSeeds.size() ;
      printWeights();
    };

    vector < CachingVertex<5> > ret;
    for ( vector< pair < int, CachingVertex<5> > >::const_iterator 
          i=theVertexSeeds.begin(); i!=theVertexSeeds.end() ; ++i )
    {
      ret.push_back ( theSmoother->smooth ( i->second ) );
    };

    clear();
    return ret;
  } catch ( VertexException & e ) {
    edm::LogError("MultiVertexFitter" ) << "couldnt fit: " 
                                        << theVertexSeeds.size();
    clear();
    return vector < CachingVertex<5> >();
  }
}

void MultiVertexFitter::printWeights ( const reco::TransientTrack & t ) const
{
    // LogDebug("MultiVertexFitter") << "Trk " << t.id();
    for ( vector < pair < int, CachingVertex<5> > >::const_iterator seed=theVertexSeeds.begin();
          seed!=theVertexSeeds.end(); ++seed )
    {
      LogDebug("::printWeights") << "  -- Vertex[" << seed->first << "] with " << setw(12)
           << setprecision(3) << theWeights[t][seed->first];
    };
    LogDebug("::printWeights") ;
}

void MultiVertexFitter::printWeights() const
{
  LogDebug("::printWeights") << endl << "Weight table: " << endl << "=================" ;
  for ( set < TransientTrack >::const_iterator trk=thePrimaries.begin();
        trk!=thePrimaries.end() ; ++trk )
  {
    printWeights ( *trk );
  };
  for ( vector< TransientTrack >::const_iterator trk=theTracks.begin();
        trk!=theTracks.end() ; ++trk )
  {
    printWeights ( *trk );
  };
}

void MultiVertexFitter::printSeeds() const
{
  LogDebug("::printSeeds") << endl << "Seed table: " << endl << "=====================" ;
  /*
  for ( vector < pair < int, CachingVertex<5> > >::const_iterator seed=theVertexSeeds.begin();
        seed!=theVertexSeeds.end(); ++seed )
  {
    LogDebug("MultiVertexFitter") << "  Vertex[" << TransientVertexNamer().name(seed->second) << "] at "
         << seed->second.position() ;
  };*/
}

void MultiVertexFitter::lostVertexClaimer()
{
  if ( !theReviveBelow < 0. ) return;
  // this experimental method is used to get almost lost vertices
  // back into the play by upweighting vertices with very low total weights

  bool has_revived = false;
  // find out about total weight
  for ( vector< pair < int, CachingVertex<5> > >::const_iterator i=theVertexSeeds.begin();
        i!=theVertexSeeds.end() ; ++i )
  {
    double totweight=0.;
    for ( vector< TransientTrack >::const_iterator trk=theTracks.begin();
          trk!=theTracks.end() ; ++trk )
    {
      totweight+=theWeights[*trk][i->first];
    };

    /*
    LogDebug("::lostVertexClaimer") << "[MultiVertexFitter] vertex seed " << TransientVertexNamer().name(*i)
         << " total weight=" << totweight ;*/

    if ( totweight < theReviveBelow && totweight > 0.0 )
    {
      LogDebug("::lostVertexClaimer") << "[MultiVertexFitter] now trying to revive vertex"
           << " revive_below=" << theReviveBelow ;
      has_revived=true;
      for ( vector< TransientTrack >::const_iterator trk=theTracks.begin();
            trk!=theTracks.end() ; ++trk )
      {
        theWeights[*trk][i->first]/=totweight;
      };
    };
  };
  if ( has_revived && verbose() ) printWeights();
}
