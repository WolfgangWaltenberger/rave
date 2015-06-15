#include "RaveBase/Converters/interface/RaveToAlgebraicObjects.h"
#include "RaveBase/RaveInterface/rave/Vector6D.h"
#include "RaveBase/RaveInterface/rave/Covariance6D.h"
#include "TrackingTools/TrajectoryParametrization/interface/CartesianTrajectoryError.h"

// using namespace std;

AlgebraicSymMatrix66 RaveToAlgebraicObjects::convert ( const rave::Covariance6D & err ) const
{
    AlgebraicSymMatrix66 cov6D; // ( 6, 0 );
    cov6D(0,0) = err.dxx();
    cov6D(0,1) = err.dxy();
    cov6D(0,2) = err.dxz();
    cov6D(0,3) = err.dxpx();
    cov6D(0,4) = err.dxpy();
    cov6D(0,5) = err.dxpz();
    cov6D(1,1) = err.dyy();
    cov6D(1,2) = err.dyz();
    cov6D(1,3) = err.dypx();
    cov6D(1,4) = err.dypy();
    cov6D(1,5) = err.dypz();
    cov6D(2,2) = err.dzz();
    cov6D(2,3) = err.dzpx();
    cov6D(2,4) = err.dzpy();
    cov6D(2,5) = err.dzpz();
    cov6D(3,3) = err.dpxpx();
    cov6D(3,4) = err.dpxpy();
    cov6D(3,5) = err.dpxpz();
    cov6D(4,4) = err.dpypy();
    cov6D(4,5) = err.dpypz();
    cov6D(5,5) = err.dpzpz();
    return cov6D;
}

AlgebraicSymMatrix33 RaveToAlgebraicObjects::convert ( const rave::Covariance3D & err ) const
{
    AlgebraicSymMatrix33 cov; // ( 3, 0 );
    cov(0,0) = err.dxx();
    cov(0,1) = err.dxy();
    cov(0,2) = err.dxz();
    cov(1,1) = err.dyy();
    cov(1,2) = err.dyz();
    cov(2,2) = err.dzz();
    return cov;
}

AlgebraicVector3 RaveToAlgebraicObjects::convert ( const rave::Vector3D & v ) const
{
  AlgebraicVector3 ret; // (3);
  ret(0)=v.x();
  ret(1)=v.y();
  ret(2)=v.z();
  return ret;
}
