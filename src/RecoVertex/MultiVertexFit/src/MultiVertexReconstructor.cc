#include "RecoVertex/MultiVertexFit/interface/MultiVertexReconstructor.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

namespace {
  struct CompareTwoTracks {
    int operator() ( const reco::TransientTrack & a, const reco::TransientTrack & b ) {
            return ( a.impactPointState().globalMomentum().perp() >
                     b.impactPointState().globalMomentum().perp() ) ;
    };
  };

  typedef MultiVertexFitter::TrackAndWeight TrackAndWeight;

  int verbose()
  {
    return 0;
  }

  void remove ( vector < TransientVertex > & vtces,
                const vector < reco::TransientTrack > & trks )
  {
    LogDebug("MultiVertexReconstructor") << "[MultiVertexReconstructor] fixme remove not yet implemented";
    // remove trks from vtces
  }

  vector < vector < TrackAndWeight > > recover (
      const vector < TransientVertex > & vtces, const vector < reco::TransientTrack > & trks )
  {
    set < reco::TransientTrack > st;
    for ( vector< reco::TransientTrack >::const_iterator i=trks.begin();
          i!=trks.end() ; ++i )
    {
      st.insert ( *i );
    }

    vector < vector < TrackAndWeight > > bundles;
    for ( vector< TransientVertex >::const_iterator vtx=vtces.begin();
          vtx!=vtces.end() ; ++vtx )
    {
      vector < reco::TransientTrack > trks = vtx->originalTracks();
      vector < TrackAndWeight > tnws;
      for ( vector< reco::TransientTrack >::const_iterator trk=trks.begin();
            trk!=trks.end() ; ++trk )
      {
        float w = vtx->trackWeight ( *trk );
        if ( w > 1e-5 )
        {
          TrackAndWeight tmp ( *trk, w );
          set < reco::TransientTrack >::iterator pos = st.find( *trk );
          if ( pos != st.end() )
          {
            st.erase ( pos  );
          }
          tnws.push_back ( tmp );
        };
      };
      bundles.push_back ( tnws );
    };

    if ( bundles.size() == 0 ) return bundles;

    // now add not-yet assigned tracks, with a tiny weight
    for ( set < reco::TransientTrack >::const_iterator i=st.begin();
          i!=st.end() ; ++i )
    {
      TrackAndWeight tmp ( *i, 1e-5 );
      bundles[0].push_back ( tmp );
    }
    return bundles;
  }
}

MultiVertexReconstructor::MultiVertexReconstructor (
    const VertexReconstructor & o, const AnnealingSchedule & s, 
    const VertexSmoother<5> & sm, float revive  ) :
    theSeedingReconstructor ( o.clone() ), theFitter ( new
        MultiVertexFitter ( s, DefaultLinearizationPointFinder(), sm, revive ) )
{
}

MultiVertexReconstructor::~MultiVertexReconstructor()
{
  delete theSeedingReconstructor;
  delete theFitter;
}

MultiVertexReconstructor * MultiVertexReconstructor::clone() const
{
  return new MultiVertexReconstructor ( * this );
}

MultiVertexReconstructor::MultiVertexReconstructor (
    const MultiVertexReconstructor & o ) :
  theSeedingReconstructor ( o.theSeedingReconstructor->clone() ),
  theFitter ( new MultiVertexFitter ( *(o.theFitter ) ) )
{}

vector < TransientVertex > MultiVertexReconstructor::vertices (
    const vector < reco::TransientTrack > & trks,
    const reco::BeamSpot & s ) const
{
  return fit ( trks, vector < reco::TransientTrack >(), s,
                    false /* no primaries */, true /* beamspot */ );
}

vector < TransientVertex > MultiVertexReconstructor::vertices (
    const vector < reco::TransientTrack > & trks,
    const reco::BeamSpot & s,
    const reco::TransientTrack & ghost_track ) const
{
  // cout << "[MultiVertexReconstructor] bla ghost_Track!" << endl;
  return fit ( trks, vector < reco::TransientTrack >(), s,
                    false /* no primaries */, true /* beamspot */ );
}

vector < TransientVertex > MultiVertexReconstructor::vertices (
    const vector < reco::TransientTrack > & trks,
    const vector < reco::TransientTrack > & primaries,
    const reco::BeamSpot & s ) const
{
  return fit ( trks, primaries, s, true, true );
}

vector < TransientVertex > MultiVertexReconstructor::vertices (
    const vector < reco::TransientTrack > & trks,
    const vector < reco::TransientTrack > & primaries,
    const reco::BeamSpot & s,
    const reco::TransientTrack & ghost_track) const
{
  return fit ( trks, primaries, s, true, true );
}

vector < TransientVertex > MultiVertexReconstructor::vertices (
    const vector < reco::TransientTrack > & trks ) const
{
  return fit ( trks, vector < reco::TransientTrack > (), reco::BeamSpot(),
       false /* no primaries */, false /* no spot */ );
}

vector < TransientVertex > MultiVertexReconstructor::vertices (
    const vector < reco::TransientTrack > & trks,
    const vector < reco::TransientTrack > & primaries ) const
{
  return fit ( trks, primaries, reco::BeamSpot(), true /* got primaries */,
      false /* no spot */ );
}

vector <reco::TransientTrack > MultiVertexReconstructor::collecttracks (
    const vector < reco::TransientTrack > & trks,
    const vector < reco::TransientTrack > & primaries, bool gotprimaries ) const
{
 LogDebug("MultiVertexReconstructor") << "[MultiVertexReconstructor] Function collecttracs is used! Shouldn't be used!";
 map < reco::TransientTrack, bool > st; // make sure we do not add a track twice

  vector < reco::TransientTrack > total = trks;
    for ( vector< reco::TransientTrack >::const_iterator i=trks.begin();
        i!=trks.end() ; ++i )
  {
    st[(*i)]=true;
  }

if(gotprimaries) {
    LogDebug ("::fit") << "Test in Reconstructor - in if(gotprimaries)!";
    for ( vector< reco::TransientTrack >::const_iterator i=primaries.begin();
        i!=primaries.end() ; ++i )
    {
	    if (!(st[(*i)]))
	    {
	      total.push_back ( *i );
	    }
    }
  }
  return total;
}


vector < TransientVertex > MultiVertexReconstructor::fit (
    const vector < reco::TransientTrack > & ptrks,
    const vector < reco::TransientTrack > & pprimaries,
    const reco::BeamSpot & s, bool gotprimaries, bool usespot ) const
{
  vector < reco::TransientTrack > trks = ptrks;
  vector < reco::TransientTrack > primaries = pprimaries;
  sort ( trks.begin(), trks.end(), CompareTwoTracks() );
  sort ( primaries.begin(), primaries.end(), CompareTwoTracks() );
  /* map < reco::TransientTrack, bool > st; // make sure we do not add a track twice

  vector < reco::TransientTrack > total = trks;
    for ( vector< reco::TransientTrack >::const_iterator i=trks.begin();
        i!=trks.end() ; ++i )
  {
    st[(*i)]=true;
  }

  if(gotprimaries) {
    LogDebug ("::fit") << "Test in Reconstructor - in if(gotprimaries)!";
    for ( vector< reco::TransientTrack >::const_iterator i=primaries.begin();
        i!=primaries.end() ; ++i )
    {
	    if (!(st[(*i)]))
	    {
	      total.push_back ( *i );
	    }
    }
  }*/

  vector < TransientVertex > tmp;

  if(usespot) {
    tmp = theSeedingReconstructor->vertices ( primaries, trks, s );
  } else {
    vector < reco::TransientTrack > total = collecttracks(trks, primaries, true);
    tmp = theSeedingReconstructor->vertices ( total);// FIXME! Shouldn't take all tracks in one container
  }
  //return tmp;

  LogDebug("::fit") << "seeder found " << tmp.size() << " vertices.";
  LogDebug("::fit") << "do we use the beamspot? " << usespot << " have we got primaries? " << gotprimaries;

  vector < vector < TrackAndWeight > > rc = recover ( tmp, trks );
  vector < CachingVertex<5> > cvts;

  if( usespot ) {
    LogDebug("::fit") << "[MultiVertexReconstructor] feeding the fitter with:";
    for ( vector< vector < TrackAndWeight > >::const_iterator i=rc.begin(); i!=rc.end() ; ++i )
    {
      LogDebug ("::fit") << "[Vertex]";
      for ( vector< TrackAndWeight >::const_iterator j=i->begin(); j!=i->end() ; ++j )
      {
        LogDebug ("::fit") << "    -- " << j->second;
      }
    }
    cvts = theFitter->vertices ( rc, primaries, s, true );
  } else {
    cvts = theFitter->vertices ( rc, primaries );
  }

  vector < TransientVertex > ret;
  for ( vector< CachingVertex<5> >::const_iterator i=cvts.begin();
        i!=cvts.end() ; ++i )
  {
    ret.push_back ( *i );
  };
  LogDebug("::fit") << "input " << tmp.size() << " vertices, output " << ret.size() << " vertices.";
  return ret;
}

VertexReconstructor * MultiVertexReconstructor::reconstructor() const
{
  return theSeedingReconstructor;
}

