#include "RecoVertex/KinematicFitPrimitives/interface/KinematicPerigeeConversions.h"
#include "TrackingTools/TrajectoryState/interface/PerigeeConversions.h"


ExtendedPerigeeTrajectoryParameters
KinematicPerigeeConversions::extendedPerigeeFromKinematicParameters(
  const KinematicState& state, const GlobalPoint& point) const
{
 return extendedPerigeeFromKinematicParameters(
            state.kinematicParameters(),
            state.particleCharge(),
            point, state.magneticField());
}

ExtendedPerigeeTrajectoryParameters 
KinematicPerigeeConversions::extendedPerigeeFromKinematicParameters(
  const KinematicParameters & parameters, const TrackCharge & charge, 
  const GlobalPoint & referencePoint, const MagneticField* field) const
{
  GlobalVector impactDistance = parameters.position() - referencePoint;
  double theta = parameters.momentum().theta();
  double phi = parameters.momentum().phi();
  double pt = parameters.momentum().transverse();
  double f = field->inInverseGeV(parameters.position()).z();

//making a proper sign for epsilon
  double positiveMomentumPhi  = ((phi > 0) ? phi : (2 * M_PI + phi));
  double positionPhi = impactDistance.phi();
  double positivePositionPhi =
        ((positionPhi > 0) ? positionPhi : (2 * M_PI + positionPhi));
  double phiDiff = positiveMomentumPhi - positivePositionPhi;
  if (phiDiff < 0.0) 
    phiDiff += (2 * M_PI);
  double signEpsilon = ((phiDiff > M_PI) ? -1.0 : 1.0);

  double epsilon = signEpsilon * impactDistance.perp();

  double signTC = - charge;
  bool isCharged = (signTC != 0);

  AlgebraicVector6 res;

  if (isCharged) {
    res[0] = f / pt * signTC;
  } else {
    res[0] = 1 / pt;
  }
  res[1] = theta;
  res[2] = phi;
  res[3] = epsilon;
  res[4] = impactDistance.z();
  res[5] = parameters.vector()[6];
  return ExtendedPerigeeTrajectoryParameters(res, charge);
}

ExtendedPerigeeTrajectoryError 
KinematicPerigeeConversions::extendedPerigeeErrorFromKinematicParametersError(
    const KinematicParametersError& error,
    const KinematicParameters& parameters,
    const TrackCharge& charge,
    const GlobalPoint& referencePoint,
    const MagneticField* field) const
{
  AlgebraicSymMatrix77 input = error.matrix();
  AlgebraicMatrix67 jacobian =
    jacobianCartesianToPerigee(parameters, charge, referencePoint, field);
  AlgebraicSymMatrix66 output = ROOT::Math::Similarity(jacobian, input);
  return ExtendedPerigeeTrajectoryError(output);
}

KinematicParameters KinematicPerigeeConversions::kinematicParametersFromExPerigee(
	const ExtendedPerigeeTrajectoryParameters& pr, const GlobalPoint& point,
	const MagneticField* field) const
{
 AlgebraicVector7 par;
 AlgebraicVector6 theVector = pr.vector();
 double pt;
 if (pr.charge() !=0) {
  pt = std::abs(field->inInverseGeV(point).z() / theVector[0]);
 } else {
  pt = 1. / theVector[0];
 }
 par(0) = theVector[3]*sin(theVector[2])+point.x();
 par(1) = -theVector[3]*cos(theVector[2])+point.y();
 par(2) = theVector[4]+point.z();
 par(3) = cos(theVector[2]) * pt;
 par(4) = sin(theVector[2]) * pt;
 par(5) = pt/tan(theVector[1]);
 par(6) = theVector[5];

 return KinematicParameters(par);
}

 KinematicParametersError 
 KinematicPerigeeConversions::kinematicParametersErrorFromExPerigee(
     const ExtendedPerigeeTrajectoryError& error,
     const ExtendedPerigeeTrajectoryParameters& parameters,
     const GlobalPoint& referencePoint,
     const MagneticField* field) const
{
  AlgebraicSymMatrix66 input = error.covarianceMatrix();
  AlgebraicMatrix76 jacobian = 
    jacobianPerigeeToCartesian(parameters, referencePoint, field);
  AlgebraicSymMatrix77 output = ROOT::Math::Similarity(jacobian, input);
  return KinematicParametersError(output);
}

KinematicState
KinematicPerigeeConversions::kinematicState(const AlgebraicVector4& momentum,
	const GlobalPoint& referencePoint, const TrackCharge& charge,
	const AlgebraicSymMatrix77& theCovarianceMatrix, const MagneticField* field) const
{
 AlgebraicMatrix77 param2cart = jacobianParameters2Kinematic(momentum,
				referencePoint, charge, field);
 AlgebraicSymMatrix77 kinematicErrorMatrix = ROOT::Math::Similarity(param2cart,theCovarianceMatrix);
//  kinematicErrorMatrix.assign(param2cart*theCovarianceMatrix*param2cart.T());

 KinematicParametersError kinematicParamError(kinematicErrorMatrix);
 AlgebraicVector7 par;
 AlgebraicVector4 mm = momentumFromPerigee(momentum, referencePoint, charge, field);
 par(0) = referencePoint.x();
 par(1) = referencePoint.y();
 par(2) = referencePoint.z();
 par(3) = mm(0);
 par(4) = mm(1);
 par(5) = mm(2);
 par(6) = mm(3);
 KinematicParameters kPar(par);
 return KinematicState(kPar, kinematicParamError, charge, field);
}

AlgebraicMatrix77 KinematicPerigeeConversions::jacobianParameters2Kinematic(
	const AlgebraicVector4& momentum, const GlobalPoint& referencePoint,
	const TrackCharge& charge, const MagneticField* field)const
{
  PerigeeConversions pc;
  AlgebraicMatrix66 param2cart = pc.jacobianParameters2Cartesian
  	(AlgebraicVector3(momentum[0],momentum[1],momentum[2]),
	referencePoint, charge, field);
  AlgebraicMatrix77 frameTransJ;
  for (int i =0;i<6;++i)
    for (int j =0;j<6;++j)
      frameTransJ(i, j) = param2cart(i, j);
  frameTransJ(6, 6) = 1;

//   double factor = 1;
//   if (charge != 0){
//    double field = TrackingTools::FakeField::Field::inGeVPerCentimeter(referencePoint).z();
//    factor =  -field*charge;
//    }
//   AlgebraicMatrix frameTransJ(7, 7, 0);
//   frameTransJ[0][0] = 1;
//   frameTransJ[1][1] = 1;
//   frameTransJ[2][2] = 1;
//   frameTransJ[6][6] = 1;
//   frameTransJ[3][3] = - factor * cos(momentum[2])  / (momentum[0]*momentum[0]);
//   frameTransJ[4][3] = - factor * sin(momentum[2])  / (momentum[0]*momentum[0]);
//   frameTransJ[5][3] = - factor / tan(momentum[1])  / (momentum[0]*momentum[0]);
//
//   frameTransJ[3][5] = - factor * sin(momentum[1]) / (momentum[0]);
//   frameTransJ[4][5] =   factor * cos(momentum[1])  / (momentum[0]);
//   frameTransJ[5][4] = -factor/ (momentum[0]*sin(momentum[1])*sin(momentum[1]));

  return frameTransJ;

}

// Cartesian (px,py,px,m) from extended perigee
AlgebraicVector4
KinematicPerigeeConversions::momentumFromPerigee(const AlgebraicVector4& momentum,
	const GlobalPoint& referencePoint, const TrackCharge& ch,
	const MagneticField* field)const
{
 AlgebraicVector4 mm;
 double pt;
 if(ch !=0){
//   pt = abs(MagneticField::inGeVPerCentimeter(referencePoint).z() / momentum[0]);
    pt = std::abs(field->inInverseGeV(referencePoint).z() / momentum[0]);
 }else{pt = 1/ momentum[0];}
 mm(0) = cos(momentum[2]) * pt;
 mm(1) = sin(momentum[2]) * pt;
 mm(2) = pt/tan(momentum[1]);
 mm(3) = momentum[3];
 return mm;
}

AlgebraicMatrix76 KinematicPerigeeConversions::jacobianPerigeeToCartesian( 
    const ExtendedPerigeeTrajectoryParameters& parameters,
    const GlobalPoint& referencePoint,
    const MagneticField* field) const
{
  // The parameter position encoded in enums for readability
  AlgebraicVector6  params = parameters.vector();
  enum { RHO=0, THETA=1, PHIP=2, EPSILON=3, ZP=4, MP=5 };
  enum { RX=0, RY=1, RZ=2, PX=3, PY=4, PZ=5, MC=6 };

  // Calculate the transverse momentum p_{t}
  double Pt = 0;
  if (parameters.charge() == 0) {
    Pt = 1. / params[RHO];
  } else {
    Pt = std::abs(field->inInverseGeV(referencePoint).z() / params[RHO]);
  }

  // Calculate the partial derivate of p_{t} w.r.t. \rho
  double dPtdRho = 0;
  if (parameters.charge() == 0) {
    dPtdRho = -1. / ( params[RHO]*params[RHO] );
  } else {
    if (params[RHO] < 0) {
      dPtdRho = std::abs(field->inInverseGeV(referencePoint).z()  / (params[RHO] * params[RHO]));
    } else {
      dPtdRho = - std::abs(field->inInverseGeV(referencePoint).z() / (params[RHO] * params[RHO]));
    }
  }

  // Fill the jacobian
  AlgebraicMatrix76 result;
  result(RX,PHIP)    = params[EPSILON] * cos(params[PHIP]);
  result(RX,EPSILON) = sin(params[PHIP]);
  result(RY,PHIP)    = params[EPSILON] * sin(params[PHIP]);
  result(RY,EPSILON) = - cos(params[PHIP]);
  result(RZ,ZP)      = 1.;
  result(PX,RHO)     = cos(params[PHIP]) * dPtdRho;
  result(PX,PHIP)    = - Pt * sin(params[PHIP]);
  result(PY,RHO)     = sin(params[PHIP]) * dPtdRho;
  result(PY,PHIP)    = Pt * cos(params[PHIP]);
  result(PZ,RHO)     = dPtdRho / tan(params[THETA]);
  result(PZ,THETA)   = Pt * (-1. - (1. / (tan(params[THETA]) * tan(params[THETA]))));
  result(MC,MP)      = 1.;

  return result;
}

AlgebraicMatrix67 KinematicPerigeeConversions::jacobianCartesianToPerigee( 
    const KinematicParameters& parameters,
    const TrackCharge& charge,
    const GlobalPoint& referencePoint,
    const MagneticField* field) const
{
  // The parameter position encoded in enums for readability
  AlgebraicVector7  params = parameters.vector();
  enum { RX=0, RY=1, RZ=2, PX=3, PY=4, PZ=5, MC=6 };
  enum { RHO=0, THETA=1, PHIP=2, EPSILON=3, ZP=4, MP=5 };

  // Calculate the abbreviation variables
  double alpha = 0;
  if (charge == 0) {
    alpha = 1.;
  } else {
    alpha = - std::abs(field->inInverseGeV(referencePoint).z());
  }
  double pt = parameters.momentum().perp();
  GlobalVector d = parameters.position() - referencePoint;
  double dt = d.perp();
  double phip = parameters.momentum().phi();
  double phid = d.phi();
  double deltaphi = fmod(phip + 2 * M_PI, 2* M_PI) - fmod(phid + 2 * M_PI, 2* M_PI);
  double beta = 1 / (1 + pow(pt / params[PZ], 2));
  double gamma = 1 / (1 + pow(params[PY] / params[PX], 2));
  double zeta = ((deltaphi > M_PI) ? -1.0 : 1.0);

  AlgebraicMatrix67 result;
  result(RHO,PX)     = - alpha * params[PX] / pow(pt, 3);
  result(RHO,PY)     = - alpha * params[PY] / pow(pt, 3);
  result(THETA,PX)   = beta * params[PX] / (pt * params[PZ]);
  result(THETA,PY)   = beta * params[PY] / (pt * params[PZ]);
  result(THETA,PZ)   = - beta * pt / (params[PZ] * params[PZ]);
  result(PHIP,PX)    = - gamma * params[PY] / (params[PX] * params[PX]);
  result(PHIP,PY)    = gamma / params[PX];
  result(EPSILON,RX) = zeta * d.x() / dt;
  result(EPSILON,RY) = zeta * d.y() / dt;
  result(ZP,RZ)      = 1.;
  result(MP,MC)      = 1.;

  return result;
}
