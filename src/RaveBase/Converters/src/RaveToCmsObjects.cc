#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RaveBase/RaveInterface/rave/Vector6D.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/PerpendicularBoundPlaneBuilder.h"
#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"
#include "RaveBase/Converters/interface/RaveToAlgebraicObjects.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackFromFTSFactory.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


// for gsf
#ifdef HAS_GSF
#include "TrackingTools/TransientTrack/interface/GsfTransientTrack.h"
#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateAssembler.h"
// #include "TrackingTools/GsfTools/interface/MultiTrajectoryStateCombiner.h"
// #include "DataFormats/GeometrySurface/interface/Cylinder.h"
#endif

#include <iostream>
#include <vector>

using namespace std;

namespace {
  // 
}

GlobalTrajectoryParameters RaveToCmsObjects::convert ( const rave::Track & s ) const
{
  return convert( s.state(), s.charge() );
}

GlobalTrajectoryParameters RaveToCmsObjects::convert(const rave::Vector6D & state, const rave::Charge & charge) const
{
  GlobalPoint  position ( state.x(),  state.y(),  state.z() );
  GlobalVector momentum ( state.px(), state.py(), state.pz() );
  return GlobalTrajectoryParameters ( position , momentum, charge, MagneticFieldSingleton::Instance() );
}

CartesianTrajectoryError RaveToCmsObjects::convert ( const rave::Covariance6D & c ) const
{
  AlgebraicSymMatrix66 s;// ( 6, 0 );
  s(0,0) = c.dxx();
  s(0,1) = c.dxy();
  s(0,2) = c.dxz();
  s(0,3) = c.dxpx();
  s(0,4) = c.dxpy();
  s(0,5) = c.dxpz();
  s(1,1) = c.dyy();
  s(1,2) = c.dyz();
  s(1,3) = c.dypx();
  s(1,4) = c.dypy();
  s(1,5) = c.dypz();
  s(2,2) = c.dzz();
  s(2,3) = c.dzpx();
  s(2,4) = c.dzpy();
  s(2,5) = c.dzpz();
  s(3,3) = c.dpxpx();
  s(3,4) = c.dpxpy();
  s(3,5) = c.dpxpz();
  s(4,4) = c.dpypy();
  s(4,5) = c.dpypz();
  s(5,5) = c.dpzpz();
  return CartesianTrajectoryError ( s );
}


GlobalPoint RaveToCmsObjects::convert ( const rave::Vector3D & v ) const
{
  return GlobalPoint ( v.x(), v.y(), v.z() );
}

GlobalVector RaveToCmsObjects::toVector ( const rave::Vector3D & v ) const
{
  return GlobalVector ( v.x(), v.y(), v.z() );
}

AlgebraicVector RaveToCmsObjects::toAlgebraicVector ( const rave::Vector3D & v ) const
{
  AlgebraicVector av ( 3 );
  av[0] = v.x();
  av[1] = v.y();
  av[2] = v.z();
  return av;
}

AlgebraicVector3 RaveToCmsObjects::toAlgebraicVector3 ( const rave::Vector3D & v ) const
{
  AlgebraicVector3 av;
  av(0) = v.x();
  av(1) = v.y();
  av(2) = v.z();
  return av;
}


GlobalError RaveToCmsObjects::convert ( const rave::Covariance3D & c ) const
{
  return GlobalError ( c.dxx(), c.dxy(), c.dyy(), c.dxz(), c.dyz(), c.dzz() );
}

TransientVertex RaveToCmsObjects::convert ( const rave::Vertex & v ) const
{
  GlobalPoint pos = convert ( v.position() );
  GlobalError err ( convert ( v.error() ) );
  vector < reco::TransientTrack > trks;
  vector< rave::Track > orig = v.tracks();
  for ( vector< rave::Track >::const_iterator i = orig.begin(); i != orig.end() ; ++i )
  {
    trks.push_back ( tTrack ( *i ) );
  }
  return TransientVertex ( pos, err, trks, v.chiSquared() );
}

reco::TransientTrack RaveToCmsObjects::tTrack ( const rave::Track & rt ) const
{
  /*
  cout << "[RaveToCmsObjects] converting an " << rt.components().size() << " components track."
       << endl; */
  #ifdef HAS_GSF
  if ( rt.components().size() )
  {
    // edm::LogError("RaveToCmsObjects") << "cannot yet handle multi component tracks. FIXME implement me. In " << __LINE__;
    FreeTrajectoryState fts = convertTrackToFTS( rt );
    const vector < pair < float, rave::Track > > & comps=rt.components();
    MultiTrajectoryStateAssembler assembler;
    // MultiTrajectoryStateCombiner combiner;

    // vector < TrajectoryStateOnSurface > cmscomps;

    // construct a cylinder around beampipe
    // Cylinder c ( GlobalPoint ( 0., 0., 0. ), TkRotation<float>(), 0.001 );
    BoundPlane *p = PerpendicularBoundPlaneBuilder () ( fts.position(), fts.momentum() );

    // edm::LogError("RaveToCmsObjects") << comps.size() << " components.";
    for ( vector< pair < float, rave::Track > >::const_iterator i=comps.begin(); i!=comps.end() ; ++i )
    {
      FreeTrajectoryState ftsc = convertTrackToFTS( i->second );
      // FIXME no weight anymore?!?
      TrajectoryStateOnSurface tsosc ( ftsc, *p, SurfaceSideDefinition::afterSurface ); //, i->first );
      assembler.addState ( tsosc );
      // cmscomps.push_back ( tsosc );
    }

    /*
    cout << "[RaveToCmsObjects] we have have " << comps.size() << " components." << endl;
    cout << "[RaveToCmsObjects] `- combined " << assembler.combinedState ().components().size() << " components." << endl;
    */

    // reco::TransientTrack ret ( new reco::GsfTransientTrack ( assembler.combinedState() ) );
    reco::TransientTrack ret ( new reco::GsfTransientTrack ( assembler.combinedState () ) );
    /* cout << "[RaveToCmsObjects] `- produced object has " << ret.stateOnSurface ( GlobalPoint() ).components().size()
         << " components" << endl;
    cout << "[RaveToCmsObjects]      `- impactPoint: " << ret.impactPointState().components().size()
         << " components" << endl;
         */
    
    // reco::TransientTrack ret=TransientTrack(new GsfTransientTrack(*t, theField ) );
    // reco::TransientTrack ret=TransientTrackFromFTSFactory().build ( fts );
    ret.setId ( rt.id() );
    // edm::LogError("RaveToCmsObjects") << " id=" << ret.id();
    return ret;
  }
  #endif
  FreeTrajectoryState fts = convertTrackToFTS( rt );
  // GlobalPoint ref ( rt.state().x(), rt.state().y(), rt.state().z() );
  reco::TransientTrack ret=TransientTrackFromFTSFactory().build ( fts ); // , ref ); //, rt.id(), rt.chi2(), rt.ndof(), ( void * ) ( &rt ) );
  ret.setId ( rt.id() );
  return ret;
}

FreeTrajectoryState RaveToCmsObjects::convertTrackToFTS ( const rave::Track & rt ) const
{
  AlgebraicSymMatrix66 cov6D = RaveToAlgebraicObjects().convert ( rt.error() );
  GlobalTrajectoryParameters gtp = convert( rt );

  FreeTrajectoryState fts ( gtp, CartesianTrajectoryError ( cov6D ) );
  fts.setTrackId ( rt.id() );
  return fts;
}


PerigeeTrajectoryParameters RaveToCmsObjects::convert(
  const rave::PerigeeParameters5D & ps, const rave::Charge & charge) const
{
  AlgebraicVector5 av;
  av[0] = ps.rho();     // inverse curvature
  av[1] = ps.theta();
  av[2] = ps.phip();
  av[3] = ps.epsilon(); // transverse impact parameter
  av[4] = ps.zp();      // longitudinal impact parameter
  return PerigeeTrajectoryParameters(av, charge != 0);
}

PerigeeTrajectoryError RaveToCmsObjects::convert(
  const rave::PerigeeCovariance5D & pe) const
{
  AlgebraicSymMatrix55 error;
  error(0,0) = pe.drr();
  error(0,1) = pe.drt();
  error(0,2) = pe.drp();
  error(0,3) = pe.dre();
  error(0,4) = pe.drz();
  error(1,1) = pe.dtt();
  error(1,2) = pe.dtp();
  error(1,3) = pe.dte();
  error(1,4) = pe.dtz();
  error(2,2) = pe.dpp();
  error(2,3) = pe.dpe();
  error(2,4) = pe.dpz();
  error(3,3) = pe.dee();
  error(3,4) = pe.dez();
  error(4,4) = pe.dzz();
  return PerigeeTrajectoryError(error);
}

#ifdef WITH_KINEMATICS

AlgebraicVector RaveToCmsObjects::toAlgebraicVector(const rave::Vector4D & v) const
{
  AlgebraicVector av(4);
  av[0] = v.x();
  av[1] = v.y();
  av[2] = v.z();
  av[3] = v.m();
  return av;
}

::KinematicParameters RaveToCmsObjects::convert(const rave::Vector7D & v) const
{
  // AlgebraicVector av ( 7 );
  AlgebraicVector7 av;
  av[0] = v.x();
  av[1] = v.y();
  av[2] = v.z();
  av[3] = v.px();
  av[4] = v.py();
  av[5] = v.pz();
  av[6] = v.m();
  return ::KinematicParameters(av);
}

AlgebraicSymMatrix RaveToCmsObjects::toAlgebraicSymMatrix(const rave::Covariance4D & c) const
{
  AlgebraicSymMatrix s(4,0);
  s(0,0) = c.dxx();
  s(0,1) = c.dxy();
  s(0,2) = c.dxz();
  s(0,3) = c.dxm();
  s(1,1) = c.dyy();
  s(1,2) = c.dyz();
  s(1,3) = c.dym();
  s(2,2) = c.dzz();
  s(2,3) = c.dzm();
  s(3,3) = c.dmm();
  return s;
}

::KinematicParametersError RaveToCmsObjects::convert(const rave::Covariance7D & c) const
{
  // AlgebraicSymMatrix s ( 7, 0 );
  AlgebraicSymMatrix77 s;
  s(0, 0) = c.dxx();
  s(0, 1) = c.dxy();
  s(0, 2) = c.dxz();
  s(0, 3) = c.dxpx();
  s(0, 4) = c.dxpy();
  s(0, 5) = c.dxpz();
  s(0, 6) = c.dxm();
  s(1, 1) = c.dyy();
  s(1, 2) = c.dyz();
  s(1, 3) = c.dypx();
  s(1, 4) = c.dypy();
  s(1, 5) = c.dypz();
  s(1, 6) = c.dym();
  s(2, 2) = c.dzz();
  s(2, 3) = c.dzpx();
  s(2, 4) = c.dzpy();
  s(2, 5) = c.dzpz();
  s(2, 6) = c.dzm();
  s(3, 3) = c.dpxpx();
  s(3, 4) = c.dpxpy();
  s(3, 5) = c.dpxpz();
  s(3, 6) = c.dpxm();
  s(4, 4) = c.dpypy();
  s(4, 5) = c.dpypz();
  s(4, 6) = c.dpym();
  s(5, 5) = c.dpzpz();
  s(5, 6) = c.dpzm();
  s(6, 6) = c.dmm();
  return ::KinematicParametersError(s);
}

ExtendedPerigeeTrajectoryParameters RaveToCmsObjects::convert(
    const rave::PerigeeParameters6D & ps, const rave::Charge & charge) const
{
  AlgebraicVector6 av;
  av[0] = ps.rho();     // inverse curvature
  av[1] = ps.theta();
  av[2] = ps.phip();
  av[3] = ps.epsilon(); // transverse impact parameter
  av[4] = ps.zp();      // longitudinal impact parameter
  av[5] = ps.mass();
  return ExtendedPerigeeTrajectoryParameters(av, charge != 0);
}

Plane RaveToCmsObjects::convert ( const ravesurf::Plane & r ) const
{
  GlobalPoint pos = convert ( r.position()  );
  // now convert r.normalVector() to a TkRotation thingy.
  // TkRotation<float> rot;
  PerpendicularBoundPlaneBuilder builder;
  return *(builder( pos, toVector ( r.normalVector() ) ) );
  // return Plane ( pos, rot );
}

Cylinder RaveToCmsObjects::convert ( const ravesurf::Cylinder & c ) const
{
  GlobalVector axis = toVector ( c.axis() );
  if (  axis.x() != 0. || axis.y() != 0. )
  {
    cout << "[RaveToCmsObjects] error: cylinder arent yet rotated. Need to implement."
         << endl;
  }
  GlobalPoint pos = convert ( c.position() );
  // now convert r.axis() to a TkRotation thingy
  TkRotation<float> rot;
  float radius = c.radius();
  return Cylinder ( pos, rot, radius );
}

ExtendedPerigeeTrajectoryError RaveToCmsObjects::convert(
    const rave::PerigeeCovariance6D & pe) const
{
  AlgebraicSymMatrix66 error;
  error(0,0) = pe.drr();
  error(0,1) = pe.drt();
  error(0,2) = pe.drp();
  error(0,3) = pe.dre();
  error(0,4) = pe.drz();
  error(0,5) = pe.drm();
  error(1,1) = pe.dtt();
  error(1,2) = pe.dtp();
  error(1,3) = pe.dte();
  error(1,4) = pe.dtz();
  error(1,5) = pe.dtm();
  error(2,2) = pe.dpp();
  error(2,3) = pe.dpe();
  error(2,4) = pe.dpz();
  error(2,5) = pe.dpm();
  error(3,3) = pe.dee();
  error(3,4) = pe.dez();
  error(3,5) = pe.dem();
  error(4,4) = pe.dzz();
  error(4,5) = pe.dzm();
  error(5,5) = pe.dmm();
  return ExtendedPerigeeTrajectoryError(error);
}

#endif

