#include "RaveBase/Converters/interface/RaveToPerigeeObjects.h"

#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RaveBase/Converters/interface/CmsToRaveObjects.h"

#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicPerigeeConversions.h"
#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"

namespace rave
{

PerigeeParameters3D RaveToPerigeeObjects::convert(
  const Vector3D & momentum, const Charge charge) const
{
  /* This is done quite nasty by using the momentum information to build a
   * state at the origin, which only works for homogenous magnetic fields.
   */

  Vector6D euclidianState(0., 0., 0., momentum.x(), momentum.y(), momentum.z());
  PerigeeParameters5D perigeeParameters = convert(euclidianState, charge);

  return PerigeeParameters3D(perigeeParameters.rho(), perigeeParameters.theta(),
                             perigeeParameters.phip());
}

PerigeeParameters5D RaveToPerigeeObjects::convert(
  const Vector6D & state, const Charge charge,
  const Point3D & referencePoint) const
{
  RaveToCmsObjects frameworkInputConverter;
  FreeTrajectoryState fts(frameworkInputConverter.convert(state, charge));
  GlobalPoint ref = frameworkInputConverter.convert(referencePoint);

  PerigeeConversions parameterConverter;
  double transverseMomentum;
  PerigeeTrajectoryParameters ptp =
    parameterConverter.ftsToPerigeeParameters(fts, ref, transverseMomentum);

  CmsToRaveObjects frameworkOutputConverter;
  return frameworkOutputConverter.convert(ptp);
}

PerigeeCovariance5D RaveToPerigeeObjects::convert(
  const Covariance6D & error, const Vector6D & state, const Charge charge,
  const Point3D & referencePoint) const
{
  RaveToCmsObjects frameworkInputConverter;
  FreeTrajectoryState fts(frameworkInputConverter.convert(state, charge),
                          frameworkInputConverter.convert(error));
  GlobalPoint ref = frameworkInputConverter.convert(referencePoint);

  PerigeeConversions parameterConverter;
  double transverseMomentum;
  /* PerigeeTrajectoryParameters ptp =
    parameterConverter.ftsToPerigeeParameters(fts, ref, transverseMomentum);
    */
  PerigeeTrajectoryError pte =
    parameterConverter.ftsToPerigeeError(fts);

  CmsToRaveObjects frameworkOutputConverter;

  // It would be cheap to use the PerigeeTrajectoryParameters twice and return the
  // PerigeeParameters5D as well, but that could let SWIG stumble.
  return frameworkOutputConverter.convert(pte);
}

#ifdef WITH_KINEMATICS

PerigeeParameters6D RaveToPerigeeObjects::convert(
  const Vector7D & parameters, const Charge charge,
  const Point3D & referencePoint) const
{
  // Convert input to internal data structures
  RaveToCmsObjects frameworkInputConverter;
  GlobalPoint ref =
    frameworkInputConverter.convert(referencePoint);

  /*
  // Propagate the parameters to the point closest to the reference point
  FreeTrajectoryState fts(frameworkInputConverter.convert(parameters, charge));
  TrajectoryStateClosestToPoint tscp =
    TSCPBuilderNoMaterial()(fts, ref);

  GlobalPoint  position = tscp.position();
  GlobalVector momentum = tscp.momentum();
  AlgebraicVector7 av;
  av[0] = position.x();
  av[1] = position.y();
  av[2] = position.z();
  av[3] = momentum.x();
  av[4] = momentum.y();
  av[5] = momentum.z();
  av[6] = parameters.m();
  KinematicParameters input(av); 
  */
  KinematicParameters input = frameworkInputConverter.convert(parameters);

  KinematicPerigeeConversions parameterConverter;
  ExtendedPerigeeTrajectoryParameters eptp =
    parameterConverter.extendedPerigeeFromKinematicParameters(
      input, charge, ref, MagneticFieldSingleton::Instance());

  CmsToRaveObjects frameworkOutputConverter;

  // It would be cheap to use the PerigeeTrajectoryParameters twice and return the
  // PerigeeParameters5D as well, but that could let SWIG stumble.
  return frameworkOutputConverter.convert(eptp);
}

PerigeeCovariance6D RaveToPerigeeObjects::convert(
  const Covariance7D & covariance, const Vector7D & parameters,
  const Charge charge, const Point3D & referencePoint) const
{
  RaveToCmsObjects frameworkInputConverter;

  GlobalPoint ref =
    frameworkInputConverter.convert(referencePoint);

  /*
  // Propagate the parameters to the point closest to the reference point
  FreeTrajectoryState fts(frameworkInputConverter.convert(parameters, charge),
                          frameworkInputConverter.convert(static_cast<Covariance6D>(covariance)));
  TrajectoryStateClosestToPoint tscp =
      TSCPBuilderNoMaterial()(fts, ref);

  FreeTrajectoryState propagated = tscp.theState();

  GlobalPoint  position = propagated.position();
  GlobalVector momentum = propagated.momentum();
  AlgebraicVector7 av;
  av[0] = position.x();
  av[1] = position.y();
  av[2] = position.z();
  av[3] = momentum.x();
  av[4] = momentum.y();
  av[5] = momentum.z();
  av[6] = parameters.m();
  KinematicParameters internalParams(av);

  CmsToRaveObjects frameworkOutputConverter;

  Covariance7D
    propagated_error(frameworkOutputConverter.convert(propagated.cartesianError()),
                     0., 0., 0., 0., 0., 0., covariance.dmm());

  KinematicParametersError input =
      frameworkInputConverter.convert(propagated_error);
  */
  KinematicParameters internalParams = frameworkInputConverter.convert(parameters);
  KinematicParametersError input =
      frameworkInputConverter.convert(covariance);

  KinematicPerigeeConversions parameterConverter;
  ExtendedPerigeeTrajectoryError epte =
    parameterConverter.extendedPerigeeErrorFromKinematicParametersError(
      input, internalParams, charge, ref, MagneticFieldSingleton::Instance());

  CmsToRaveObjects frameworkOutputConverter;
  // It would be cheap to use the PerigeeTrajectoryParameters twice and return the
  // PerigeeParameters6D as well, but that could let SWIG stumble.
  return frameworkOutputConverter.convert(epte);
}

#endif

} // namespace rave
