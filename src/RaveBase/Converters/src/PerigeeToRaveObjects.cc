#include "RaveBase/Converters/interface/PerigeeToRaveObjects.h"

#include "RaveBase/Converters/interface/RaveToCmsObjects.h"
#include "RaveBase/Converters/interface/CmsToRaveObjects.h"
#include "RaveBase/Converters/interface/RaveToPerigeeObjects.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"

#ifdef WITH_KINEMATICS
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicPerigeeConversions.h"
#endif

#include "boost/cast.hpp"

namespace rave
{

Vector3D PerigeeToRaveObjects::convert(const PerigeeParameters3D &) const
{
  // TODO
  edm::LogError("PerigeeToRaveObjects") << "Missing implementation!";
  exit(0);
}

Vector6D PerigeeToRaveObjects::convert(
  const PerigeeParameters5D & state, const Charge charge,
  const Point3D & referencePoint) const
{
  RaveToCmsObjects frameworkInputConverter;
  PerigeeTrajectoryParameters ptp = frameworkInputConverter.convert(state, charge);
  GlobalPoint ref = frameworkInputConverter.convert(referencePoint);

  // We need the charge knowledge to know from the transverseCurvature what pt is.
  float pt;
  float field = MagneticFieldSingleton::Instance()->inInverseGeV(ref).z();
  float signTC = boost::numeric_cast<float>(- charge);
  bool isCharged = (charge != 0);
  if (isCharged)
  {
    pt = field / ptp.transverseCurvature() * signTC;
  }
  else
  {
    pt = 1. / ptp.transverseCurvature();
  }

  PerigeeConversions parameterConverter;
  GlobalPoint  position = parameterConverter.positionFromPerigee(ptp, ref);
  GlobalVector momentum = parameterConverter.momentumFromPerigee(ptp, pt, ref);

  CmsToRaveObjects frameworkOutputConverter;
  return Vector6D(frameworkOutputConverter.convert(position),
                  frameworkOutputConverter.convert(momentum));
}

Covariance6D PerigeeToRaveObjects::convert(
  const PerigeeCovariance5D & error, const Vector6D & state,
  const Charge charge) const
{
  // PerigeeParameters5D ps = RaveToPerigeeObjects().convert(state, charge);

  RaveToCmsObjects frameworkInputConverter;
  GlobalTrajectoryParameters gtp =
    frameworkInputConverter.convert(state, charge);
  PerigeeTrajectoryError pte =
    frameworkInputConverter.convert(error);

  PerigeeConversions parameterConverter;
  CurvilinearTrajectoryError cte =
    parameterConverter.curvilinearError(pte, gtp);

  // The FreeTrajectoryState is used for conversion curvilinear -> cartesian
  CmsToRaveObjects frameworkOutputConverter;
  return frameworkOutputConverter.convert(
           FreeTrajectoryState(gtp, cte).cartesianError());
}

#ifdef WITH_KINEMATICS
Vector7D PerigeeToRaveObjects::convert(
  const PerigeeParameters6D & parameters, const Charge charge,
  const Point3D & referencePoint) const
{
  /*
  Vector6D sixD = convert(parameters, charge, referencePoint);
  return Vector7D(sixD, parameters.mass());
  */

  RaveToCmsObjects frameworkInputConverter;
  ExtendedPerigeeTrajectoryParameters eptp =
      frameworkInputConverter.convert(parameters, charge);
  GlobalPoint ref = frameworkInputConverter.convert(referencePoint);

  KinematicPerigeeConversions parameterConverter;
  KinematicParameters internalResult =
      parameterConverter.kinematicParametersFromExPerigee(
          eptp, ref, MagneticFieldSingleton::Instance());

  CmsToRaveObjects frameworkOutputConverter;
  return frameworkOutputConverter.convert(internalResult);
}

Covariance7D PerigeeToRaveObjects::convert(
  const PerigeeCovariance6D & covariance, const Vector7D & parameters,
  const Charge charge, const Point3D & referencePoint) const
{
  PerigeeParameters6D perigeeParameters =
      RaveToPerigeeObjects().convert(parameters, charge, referencePoint);

  RaveToCmsObjects frameworkInputConverter;
  ExtendedPerigeeTrajectoryParameters eptp =
      frameworkInputConverter.convert(perigeeParameters, charge);
  ExtendedPerigeeTrajectoryError epte =
      frameworkInputConverter.convert(covariance);
  GlobalPoint ref = frameworkInputConverter.convert(referencePoint);

  KinematicPerigeeConversions parameterConverter;
  KinematicParametersError internalResult =
      parameterConverter.kinematicParametersErrorFromExPerigee(
      epte, eptp, ref, MagneticFieldSingleton::Instance());

  CmsToRaveObjects frameworkOutputConverter;
  return frameworkOutputConverter.convert(internalResult);
}
#endif

} // namespace rave
