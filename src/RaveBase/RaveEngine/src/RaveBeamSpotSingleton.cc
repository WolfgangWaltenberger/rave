#include "RaveBase/RaveEngine/interface/RaveBeamSpotSingleton.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;
using namespace rave;

namespace {
  reco::BeamSpot * mySpot = 0;
  rave::Ellipsoid3D myEllipsoid;
}

const rave::Ellipsoid3D & BeamSpotSingleton::get()
{
  return myEllipsoid;
}

const reco::BeamSpot * BeamSpotSingleton::instance()
{
  return mySpot;
}

void BeamSpotSingleton::set ( const rave::Ellipsoid3D & ell )
{
  cout << "here" << endl;
  myEllipsoid=ell;
  if ( mySpot ) delete mySpot;
  if ( !(ell.isValid()) )
  {
    mySpot=0;
    return;
  }
  math::XYZPoint p ( ell.point().x(), ell.point().y(), ell.point().z() );
  math::Error<7>::type error;
  edm::LogInfo("BeamSpotSingleton") << "Currently beamspot info is \"split up\"";
  // ... meaning  that sigmaZ is treated "systematic" as beam length,
  // while the rest of the covariance matrix enters as a "statistic" error.
  // this isnt really correct, but the distinction is (I believe) never
  // used in RAVE.

  float cxx=ell.covariance().dxx();
  float cyy=ell.covariance().dyy();
  float czz=ell.covariance().dzz();
  error(0,0)=cxx; // FIXME, need another way to do this!!!
  error(1,1)=cyy;
  /*
  error(2,2)=czz;
  */
  error(1,0)=ell.covariance().dxy();
  error(2,0)=ell.covariance().dxz();
  error(2,1)=ell.covariance().dyz();
  mySpot = new reco::BeamSpot ( p, sqrt ( czz ), 0., 0., 0. , error );
  if ( false )
  {
    edm::LogInfo("BeamSpotSingleton::set") 
    // cout 
      << " rxx[um]=" << 10000. * sqrt ( mySpot->rotatedCovariance3D()(0,0) )
      << " |rxy|[um]=" << 10000. * sqrt ( fabs ( mySpot->rotatedCovariance3D()(0,1) ) )
      << " ryy[um]=" << 10000. * sqrt ( mySpot->rotatedCovariance3D()(1,1) )
      << " rzz[um]=" << 10000. * sqrt ( mySpot->rotatedCovariance3D()(2,2) ) << endl;
  }
}

bool BeamSpotSingleton::hasBeamSpot()
{
  return ( mySpot != 0 );
}

