#include "RaveBase/Converters/interface/CmsToRaveObjects.h"
#include "RaveBase/RaveInterface/rave/Vector6D.h"
#include "RaveBase/RaveInterface/rave/Covariance6D.h"
#include "RaveBase/RaveInterface/rave/PerigeeParameters5D.h"
#include "RaveBase/RaveInterface/rave/PerigeeCovariance5D.h"
#include "TrackingTools/TrajectoryParametrization/interface/CartesianTrajectoryError.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include "RaveBase/Converters/interface/RaveStreamers.h"

#include "boost/cast.hpp"

#include <cmath>
#include <limits>
#include <ostream>
#include <set>

using namespace std;

namespace
{
  rave::Track getNr ( const vector < rave::Track > & trks, int id )
  {
    for ( vector< rave::Track >::const_iterator i = trks.begin();
          i != trks.end() ; ++i )
    {
      if ( i->id() == id ) return *i;
    }
    cout << "[CmsToRaveObjects] error: couldnt find track nr " << id 
         << " in list." << endl;
    cout << "[CmsToRaveObjects] available numbers: " << flush;
    for ( vector< rave::Track >::const_iterator i=trks.begin(); 
          i!=trks.end() ; ++i )
    {
      cout << i->id() << " ";
    }
    cout << endl;
    exit ( 0 );
  }

  struct ltVertex
  {
    bool operator() ( const pair < float, rave::Vertex > & r1,
                      const pair < float, rave::Vertex > & r2 )
    {
      return ( r1.first > r2.first );
    }
  };

  typedef numeric_limits<float> num;
}

rave::Covariance33D CmsToRaveObjects::convert ( const AlgebraicMatrix3M & m ) const
{
  //cout << endl;
  //cout << "m=" << m << endl;
  rave::Covariance33D ret ( m(0,0), m(0,1), m(2,0), m(1,0), m(1,1), m(1,2), 
                             m(2,0), m(2,1), m(2,2) );
  //cout << "ret=" << ret << endl;
  return ret;
}

rave::Track CmsToRaveObjects::convert ( const reco::TransientTrack & t, int id ) const
{
  /*
  rave::Vector6D st=convert ( t.impactPointState().globalParameters() );
  rave::Covariance6D err = convert ( t.impactPointState().cartesianParameters() );
  return rave::Track();
  */
  ostringstream ostr;
  ostr << "id" << id;
  // return convert ( t.impactPointState(), t.chi2(), t.ndof(), 0, ostr.str() );
  return convert ( id, t.initialFreeState(), t.chi2(), t.ndof(), 0, ostr.str() );
}

rave::Track CmsToRaveObjects::convert ( const TrajectoryStateOnSurface & s, float chi2,
                                        float ndf, void * p, const string & tag ) const
{
  return convert ( *(s.freeState()), chi2, ndf, p, tag );
  // FIXME change here?
  /*
  rave::Vector6D st = convert ( s.globalParameters() );
  rave::Covariance6D err = convert ( s.cartesianError() );
  return rave::Track ( st, err, s.charge(), chi2, ndf, p, tag );
  */
}

rave::Track CmsToRaveObjects::convert ( int id, const TrajectoryStateOnSurface & s, 
    float chi2, float ndf, void * p, const string & tag ) const
{
  /*
  return convert ( id, *(s.freeState()), chi2, ndf, p, tag );
  */
  if ( s.freeState()->trackId() != id )
  {
    edm::LogError ("CmsToRaveObjects" ) << " track id mismatch! " << id << " versus " << s.freeState()->trackId();
  }
  rave::Vector6D st = convert ( s.globalParameters() );
  rave::Covariance6D err = convert ( s.cartesianError() );
  if ( id != -1 )
    return rave::Track ( id, st, err, s.charge(), chi2, ndf, p, tag );
  return rave::Track ( st, err, s.charge(), chi2, ndf, p, tag );
}

rave::BasicTrack CmsToRaveObjects::toBasicTrack ( const TrajectoryStateOnSurface & s, float chi2,
                                        float ndf, void * p, const string & tag ) const
{
  rave::Vector6D st = convert ( s.globalParameters() );
  rave::Covariance6D err = convert ( s.cartesianError() );
  if ( s.freeState() && s.freeState()->trackId() != -1 )
  {
    return rave::BasicTrack ( s.freeState()->trackId(), st, err, s.charge(), chi2, ndf, p, tag );
  }
  return rave::BasicTrack ( st, err, s.charge(), chi2, ndf, p, tag );
}

rave::Track CmsToRaveObjects::convert ( const FreeTrajectoryState & s ) const
{
  rave::Vector6D st = convert ( s.parameters() );
  rave::Covariance6D err = convert ( s.cartesianError() );
  if ( s.trackId() != -1 )
  {
    return rave::Track ( s.trackId(), st, err, s.charge(), num::quiet_NaN() /*chi2*/,
                         num::quiet_NaN () /*ndof*/, 0, "" );
  }
  return rave::Track ( st, err, s.charge(), num::quiet_NaN() /*chi2*/,
                       num::quiet_NaN () /*ndof*/, 0, "" );
}

rave::Track CmsToRaveObjects::convert ( const FreeTrajectoryState & s, float chi2, float ndf, 
                                        void * p, const string & tag ) const
{
  rave::Vector6D st = convert ( s.parameters() );
  rave::Covariance6D err = convert ( s.cartesianError() );
  if ( s.trackId() != -1 )
  {
    return rave::Track ( s.trackId(), st, err, s.charge(), chi2, ndf, p, tag );
  }
  return rave::Track ( st, err, s.charge(), chi2, ndf, p, tag );
}

rave::Track CmsToRaveObjects::convert ( int id, const FreeTrajectoryState & s, float chi2, float ndf, 
                                        void * p, const string & tag ) const
{
  rave::Vector6D st = convert ( s.parameters() );
  rave::Covariance6D err = convert ( s.cartesianError() );
  if ( (s.trackId() != -1) && ( id != s.trackId() ) )
  {
    edm::LogError ( "CmsToRaveObjects" ) << "error! we get two different ids for this one rave::Track";
  }

  if ( s.trackId() == -1 )
  {
    return rave::Track ( st, err, s.charge(), chi2, ndf, p, tag );
  }
  return rave::Track ( id, st, err, s.charge(), chi2, ndf, p, tag );
}

rave::Vector6D CmsToRaveObjects::convert ( const GlobalTrajectoryParameters & p ) const
{
  try {
    return rave::Vector6D ( p.position().x(), p.position().y(), p.position().z(),
                            p.momentum().x(), p.momentum().y(), p.momentum().z() );
  } catch ( rave::CheckedFloatException & e ) {
    return rave::Vector6D (); // If e contains "nan" entries, a zero tuple will be returned
  };

}

rave::Covariance3D CmsToRaveObjects::convert ( const GlobalError & e ) const
{
  try {
    return rave::Covariance3D ( e.cxx(), e.cyx(), e.czx(),
                                e.cyy(), e.czy(), e.czz() );
  } catch ( rave::CheckedFloatException & e ) {
    return rave::Covariance3D (); // If e contains "nan" entries, a zero matrix will be returned
  };
}

rave::Covariance6D CmsToRaveObjects::convert ( const CartesianTrajectoryError & e ) const
{
  try {
    return rave::Covariance6D ( e.matrix_old() [0][0], e.matrix_old() [0][1], e.matrix_old() [0][2],
                                e.matrix_old() [1][1], e.matrix_old() [1][2], e.matrix_old() [2][2],
                                e.matrix_old() [0][3], e.matrix_old() [0][4], e.matrix_old() [0][5],
                                e.matrix_old() [1][3], e.matrix_old() [1][4], e.matrix_old() [1][5],
                                e.matrix_old() [2][3], e.matrix_old() [2][4], e.matrix_old() [2][5],
                                e.matrix_old() [3][3], e.matrix_old() [3][4], e.matrix_old() [3][5],
                                e.matrix_old() [4][4], e.matrix_old() [4][5], e.matrix_old() [5][5] );
  } catch ( rave::CheckedFloatException & e ) {
    return rave::Covariance6D (); // If e contains "nan" entries, a zero matrix will be returned
  };
}

rave::Vertex CmsToRaveObjects::convert ( const TransientVertex & tv,
    const vector < rave::Track > & rave_trks ) const
{
  // cout << "[CmsToRaveObjects] converting tv" << endl;
  set < pair < float, rave::Vertex >, ltVertex > scomponents;
  vector < VertexState > orig_comps = tv.vertexState().components();
  vector < pair < float, rave::Vertex > > components;
  if ( orig_comps.size() > 1 )
  {
    components = vector < pair < float, rave::Vertex > > ( orig_comps.size() );
    // scomponents.reserve ( orig_comps.size() );
    for ( vector< VertexState >::const_iterator i=orig_comps.begin(); 
          i!=orig_comps.end() ; ++i )
    {
      rave::Vertex tmp ( rave::Point3D ( i->position().x(), i->position().y(), i->position().z() ),
          CmsToRaveObjects().convert ( tv.positionError() ), vector < pair < float, rave::Track > > (),
          0., 0., vector < pair < float, rave::Vertex > > () );
      scomponents.insert ( pair < float, rave::Vertex > ( i->weightInMixture(), tmp ) );
    }
    //cout << "[CmsToRaveObjects] we have " << components.size() << " components" << endl;
    copy ( scomponents.begin(), scomponents.end(), components.begin() );
  }

  vector < pair < float, rave::Track > > weighted_rave_tracks;

  map<reco::TransientTrack, float > wm;
    
  vector < reco::TransientTrack > original_trks = tv.originalTracks();

  if ( tv.hasTrackWeight() ) {
    wm = tv.weightMap();
  } else {
    for ( vector< reco::TransientTrack >::const_iterator i = original_trks.begin();
          i != original_trks.end() ; ++i )
    {
      wm[*i] = 1.0;
    }
  }

  float ndf = tv.degreesOfFreedom();
  float chi2 = tv.totalChiSquared();

  for ( vector< reco::TransientTrack >::const_iterator iter = original_trks.begin();
        iter != original_trks.end() ; ++iter )
  {
    rave::Track t ( getNr ( rave_trks, iter->id() ) );
    weighted_rave_tracks.push_back (
        pair< float, rave::Track > (  wm[*iter], t ) );
        // pair< float, rave::Track > (  wm[*iter], getNr ( rave_trks, 0 ) ) );
  }

  rave::Vertex ret ( rave::Point3D ( tv.position().x(),
                                        tv.position().y(),
                                        tv.position().z() ),
                        CmsToRaveObjects().convert ( tv.positionError() ),
                        weighted_rave_tracks, ndf, chi2,
                        components );

  /* cout << "[CmsToRaveObjects] converting tv: " 
       << tv.tkToVtxCovarianceIsAvailable() << endl; */
  if ( tv.tkToVtxCovarianceIsAvailable() )
  {
    for ( vector< reco::TransientTrack >::const_iterator iter = original_trks.begin();
          iter != original_trks.end() ; ++iter )
    {
      rave::Track t ( getNr ( rave_trks, iter->id() ) );
      // cout << "[CmsToRaveObjects] FIXME need to watch for id!" << endl;
      // cout << "[CmsToRaveObjects] converting " << iter->id() << endl;
      rave::Covariance33D tkVtxCov = 
        CmsToRaveObjects().convert ( tv.tkToVtxCovariance ( *iter ) );
      // cout << "[CmsToRaveObjects] adding trk to vtx cov!" << endl;
      ret.addTrackToVertexCovariance ( t, tkVtxCov );
    }
  }

  /*
  for ( map<reco::TransientTrack, float >::iterator iter = wm.begin();
        iter != wm.end();
        iter++ )
  {
    // cout << "[CmsToRaveObjects] track: #" << iter->first.id() << endl;
    weighted_rave_tracks.push_back ( 
        pair< float, rave::Track > ( iter->second, getNr ( rave_trks, iter->first.id() ) ) );
  }*/


  if ( ! ( tv.hasRefittedTracks() ) )
  {
    // cout << "[CmsToRaveObjects] done converting tv A" << endl;
    return ret;
  }

  vector < pair < float, rave::Track > > weighted_refitted_rave_tracks;
  vector < reco::TransientTrack > refitted_cms_tracks = tv.refittedTracks();
  vector < reco::TransientTrack > original_cms_tracks = tv.originalTracks();

  if ( refitted_cms_tracks.size() > original_cms_tracks.size() )
  {
    edm::LogError ( "CmsToRaveObjects" ) << "There are more refitted tracks than original tracks! This thing will crash!";
  }

  vector < reco::TransientTrack >::const_iterator oit = original_cms_tracks.begin();

  for ( vector< reco::TransientTrack >::const_iterator i = refitted_cms_tracks.begin(); i != refitted_cms_tracks.end() ; ++i )
  {
    rave::Track t =convert ( *i );
    weighted_refitted_rave_tracks.push_back ( pair< float, rave::Track > 
        ( wm[*oit], t ) );
    oit++;
  }

  rave::Vertex ret2 ( rave::Point3D ( tv.position().x(),
                                        tv.position().y(),
                                        tv.position().z() ),
                        CmsToRaveObjects().convert ( tv.positionError() ),
                        weighted_rave_tracks, weighted_refitted_rave_tracks, ndf, chi2,
                        components );

  /*
  for ( vector< reco::TransientTrack >::const_iterator i = refitted_cms_tracks.begin(); i != refitted_cms_tracks.end() ; ++i )
  {
    rave::Track t =convert ( *i );
    if ( tv.tkToVtxCovarianceIsAvailable() )
    {
      // for the refitted tracks we dont ask for covariance matrices
      rave::Covariance3D tkVtxCov = CmsToRaveObjects().convert ( tv.tkToVtxCovariance ( *i ) );
      ret2.addTrackToVertexCovariance ( t, tkVtxCov );
    }
  }*/
  // cout << "[CmsToRaveObjects] done converting tv A" << endl;
  if ( tv.tkToVtxCovarianceIsAvailable() )
  {
    for ( vector< reco::TransientTrack >::const_iterator iter = original_trks.begin();
          iter != original_trks.end() ; ++iter )
    {
      rave::Track t ( getNr ( rave_trks, iter->id() ) );
      // cout << "[CmsToRaveObjects] FIXME need to watch for id!" << endl;
      // cout << "[CmsToRaveObjects] converting " << iter->id() << endl;
      rave::Covariance33D tkVtxCov = 
        CmsToRaveObjects().convert ( tv.tkToVtxCovariance ( *iter ) );
      //cout << "[CmsToRaveObjects] tkvtx=" << tkVtxCov << endl;
      // cout << "[CmsToRaveObjects] adding trk to vtx cov!" << endl;
      ret2.addTrackToVertexCovariance ( t, tkVtxCov );
    }
  }
  return ret2;
}

/*
rave::Vertex CmsToRaveObjects::convertCV ( const CachingVertex<5> & cv,
    const vector < rave::Track > & rave_trks ) const
{
  cout << "[CmsToRaveObjects] here we are!" << endl;
  TransientVertex tv = cv;
  rave::Vertex ret = convert ( tv, rave_trks );
  return ret;
}*/

rave::Vector3D CmsToRaveObjects::convert ( const GlobalPoint & p ) const
{
  return rave::Vector3D ( boost::numeric_cast<float> ( p.x() ),
                          boost::numeric_cast<float> ( p.y() ),
                          boost::numeric_cast<float> ( p.z() ) );
}

rave::Vector3D CmsToRaveObjects::convert ( const GlobalVector & v ) const
{
  return rave::Vector3D ( boost::numeric_cast<float> ( v.x() ),
                          boost::numeric_cast<float> ( v.y() ),
                          boost::numeric_cast<float> ( v.z() ) );
}

rave::PerigeeParameters5D CmsToRaveObjects::convert(const PerigeeTrajectoryParameters & ptp) const
{
  try {
    AlgebraicVector5 av = ptp.vector();
    return rave::PerigeeParameters5D(av[0],  // rho
                                     av[1],  // theta
                                     av[2],  // phip
                                     av[3],  // epsilon
                                     av[4]); // zp
  } catch ( rave::CheckedFloatException & e ) {
    return rave::PerigeeParameters5D (); // If e contains "nan" entries, a zero tuple will be returned
  };
}

rave::PerigeeCovariance5D CmsToRaveObjects::convert(
    const PerigeeTrajectoryError & pte) const
{
  try {
    AlgebraicSymMatrix55 error = pte.covarianceMatrix();
    return rave::PerigeeCovariance5D(
        error(0,0), error(0,1), error(0,2), error(0,3), error(0,4),
        error(1,1), error(1,2), error(1,3), error(1,4), error(2,2),
        error(2,3), error(2,4), error(3,3), error(3,4), error(4,4));
  } catch ( rave::CheckedFloatException & e ) {
    return rave::PerigeeCovariance5D (); // If e contains "nan" entries, a zero matrix will be returned
  };
}

#ifdef WITH_KINEMATICS

rave::Vector7D CmsToRaveObjects::convert ( const ::KinematicParameters & p ) const
{
  try {
    return rave::Vector7D ( CmsToRaveObjects().convert ( p.position() ),
                            CmsToRaveObjects().convert ( p.momentum() ),
                            p.vector() ( 6 ) );
  } catch ( rave::CheckedFloatException & e ) {
    return rave::Vector7D (); // If e contains "nan" entries, a zero tuple will be returned
  };
}

rave::Covariance7D CmsToRaveObjects::convert ( const ::KinematicParametersError & e ) const
{
  try {
    return rave::Covariance7D (
               e.matrix() (0,0), e.matrix() (0,1), e.matrix() (0,2),
               e.matrix() (1,1), e.matrix() (1,2), e.matrix() (2,2),
               e.matrix() (0,3), e.matrix() (0,4), e.matrix() (0,5),
               e.matrix() (1,3), e.matrix() (1,4), e.matrix() (1,5),
               e.matrix() (2,3), e.matrix() (2,4), e.matrix() (2,5),
               e.matrix() (3,3), e.matrix() (3,4), e.matrix() (3,5),
               e.matrix() (4,4), e.matrix() (4,5), e.matrix() (5,5),
               e.matrix() (0,6), e.matrix() (1,6), e.matrix() (2,6),
               e.matrix() (3,6), e.matrix() (4,6), e.matrix() (5,6),
               e.matrix() (6,6) );
  } catch ( rave::CheckedFloatException & e ) {
    return rave::Covariance7D (); // If e contains "nan" entries, a zero matrix will be returned
  };
}

rave::PerigeeParameters6D CmsToRaveObjects::convert(
  const ExtendedPerigeeTrajectoryParameters & eptp) const
{
  try {
    // Get the parameters and define their position
    AlgebraicVector6 params = eptp.vector();
    enum { RHO = 0, THETA = 1, PHIP = 2, EPSILON = 3, ZP = 4, MP = 5 };

    // Convert the 5 dimensions without mass
    AlgebraicVector5 fiveParams(params.begin(), 5);
    rave::PerigeeParameters5D fiveState =
      convert(PerigeeTrajectoryParameters(fiveParams, eptp.charge() != 0));

    // Construct the requested state from the 5D state and the mass
    return rave::PerigeeParameters6D(fiveState, params[MP]);
  } catch ( rave::CheckedFloatException & e ) {
    return rave::PerigeeParameters6D (); // If e contains "nan" entries, a zero tuple will be returned
  };
}

rave::PerigeeCovariance6D CmsToRaveObjects::convert(
    const ExtendedPerigeeTrajectoryError & epte) const
{
  try {
    AlgebraicSymMatrix66 error = epte.covarianceMatrix();
    return rave::PerigeeCovariance6D(
               error(0,0), error(0,1), error(0,2), error(0,3), error(0,4), error(0,5),
               error(1,1), error(1,2), error(1,3), error(1,4), error(1,5), error(2,2),
               error(2,3), error(2,4), error(2,5), error(3,3), error(3,4), error(3,5), 
               error(4,4), error(4,5), error(5,5));
  } catch ( rave::CheckedFloatException & e ) {
    return rave::PerigeeCovariance6D (); // If e contains "nan" entries, a zero matrix will be returned
  };
}

ravesurf::Plane CmsToRaveObjects::convert ( const Plane & c ) const
{
  rave::Point3D rp = convert ( c.position() );
  rave::Vector3D nv = convert (  c.normalVector() );
  return ravesurf::Plane( rp, nv );
}

ravesurf::Cylinder CmsToRaveObjects::convert ( const Cylinder & c ) const
{
  rave::Point3D rp = convert ( c.position() );
  cout << "[CmsToRaveObjects] currently we ignore rotations!" << endl;
  rave::Vector3D rv ( 0., 0., 1. );
  float r = c.radius();
  return ravesurf::Cylinder ( rp, rv, r );
}

#endif

