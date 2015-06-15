#if 0
#include "RecoVertex/MultiVertexFit/interface/DebuggingHarvester.h"
#include <dataharvester/Writer.h>

using namespace std;
using namespace dataharvester;

namespace {
  void add ( Tuple & t, const map < string, MultiType > & a )
  {
    for ( map < string, MultiType >::const_iterator i=a.begin(); i!=a.end() ; ++i )
    {
      t[i->first]=i->second;
    }
  }

  void add ( dataharvester::Tuple & t,
      const GlobalPoint & p, const string & name )
  {
    t["point:x"]=p.x();
    t["point:y"]=p.y();
    t["point:z"]=p.z();
    t["point:name"]=name;
    static int c=0;
    ostringstream o;
    o << "pt" << c++;
    t["point:id"]=o.str();
  }

  GlobalPoint computePos ( const GlobalTrajectoryParameters & jet,
      double s )
  {
    GlobalPoint ret = jet.position();
    ret += s * jet.momentum();
    return ret;
  }
}

DebuggingHarvester::DebuggingHarvester ( const string & f ) : theFile ( f )
{}

void DebuggingHarvester::save ( const GlobalPoint & p,
    const map < string, MultiType > & attrs, const string & name )
{
  dataharvester::Tuple t ( "addendum" );
  add ( t, p, name );
  add ( t, attrs );
  dataharvester::Writer::file ( theFile ) << t;
}

void DebuggingHarvester::save ( const GlobalPoint & p , const string & name )
{
  map < string, MultiType > attrs;
  save ( p, attrs, name );
}

void DebuggingHarvester::save ( const GlobalTrajectoryParameters & p, const string & name )
{
  static int c=0;
  ostringstream o;
  o << "A" << c++;
  dataharvester::Tuple t ( "addendum" );
  t["arrow:x"]=p.position().x();
  t["arrow:y"]=p.position().y();
  t["arrow:z"]=p.position().z();
  t["arrow:id"]=o.str();
  t["arrow:dx"]=p.momentum().x();
  t["arrow:dy"]=p.momentum().y();
  t["arrow:dz"]=p.momentum().z();
  if ( name.size() ) t["arrow:name"]=name;
  dataharvester::Writer::file ( theFile ) << t;
}

void DebuggingHarvester::save (
    const vector < ModeFinder3d::PointAndDistance > & pts, const string & name )
{
  cout << "[DebuggingHarvester] not implemented." << endl;
  for ( vector< ModeFinder3d::PointAndDistance >::const_iterator p=pts.begin();
        p!=pts.end() ; ++p )
  {
    // save ( *p, name );
  }
}

void DebuggingHarvester::save (
    const vector < Cluster1D < reco::TransientTrack > > & pts,
    const GlobalTrajectoryParameters & jet, const string & name )
{
  map < string, MultiType > attrs;
  save ( pts, jet, attrs, name );
}

void DebuggingHarvester::save (
    const vector < Cluster1D < reco::TransientTrack > > & pts,
    const GlobalTrajectoryParameters & jet,
    const map < string, MultiType > & attrs, const string & name )
{
  for ( vector< Cluster1D < reco::TransientTrack > >::const_iterator p=pts.begin();
        p!=pts.end() ; ++p )
  {
    save ( *p, jet, attrs, name );
  }
}

void DebuggingHarvester::save (
    const Cluster1D < reco::TransientTrack > & p,
    const GlobalTrajectoryParameters & jet,
    const map < string, MultiType > & attrs, const string & name )
{
  dataharvester::Tuple t ( "addendum" );
  GlobalPoint pos = computePos ( jet, p.position().value() );
  vector < const reco::TransientTrack * > trks = p.tracks();
  for ( vector < const reco::TransientTrack * >::const_iterator
        i=trks.begin(); i!=trks.end() ; ++i )
  {
    t["point:asstrk:id"]=(**i).id();
    t["point:asstrk:w"]=1.;
    t.fill("point:asstrk");
  }
  add ( t, pos, name );
  add ( t, attrs );
  dataharvester::Writer::file ( theFile ) << t;
}

void DebuggingHarvester::save (
    const Cluster1D < reco::TransientTrack > & p,
    const GlobalTrajectoryParameters & jet, const string & name )
{
  cout << "[DebuggingHarvester] sacing point" << endl;
  map < string, MultiType > attrs;
  attrs["point:color"]="red";
  attrs["point:d"]=p.weight();
  save ( p, jet, attrs, name );
}

#endif
