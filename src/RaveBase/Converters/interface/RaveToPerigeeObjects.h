#include "RaveBase/RaveInterface/rave/Charge.h"
#include "RaveBase/RaveInterface/rave/Vector3D.h"
#include "RaveBase/RaveInterface/rave/Vector6D.h"
#include "RaveBase/RaveInterface/rave/Covariance6D.h"
#include "RaveBase/RaveInterface/rave/PerigeeParameters3D.h"
#include "RaveBase/RaveInterface/rave/PerigeeParameters5D.h"
#include "RaveBase/RaveInterface/rave/PerigeeCovariance5D.h"

#ifdef WITH_KINEMATICS
#include "RaveBase/RaveInterface/rave/Vector7D.h"
#include "RaveBase/RaveInterface/rave/Covariance7D.h"
#include "RaveBase/RaveInterface/rave/PerigeeParameters6D.h"
#include "RaveBase/RaveInterface/rave/PerigeeCovariance6D.h"
#endif

namespace rave
{

/**
 * @class RaveToPerigeeObjects converts from euclidian space to perigee coords.
 *
 * This conversion class is used internally by several data classes to provide
 * their data in perigee parametrization.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class RaveToPerigeeObjects
{
  public:
    PerigeeParameters3D convert(const Vector3D &, const Charge) const;
    PerigeeParameters5D convert(
      const Vector6D &, const Charge,
      const Point3D & referencePoint = Point3D(0.,0.,0.)) const;
    PerigeeCovariance5D convert(
      const Covariance6D & error, const Vector6D & state, const Charge charge,
      const Point3D & referencePointconst = Point3D(0.,0.,0.)) const;

  #ifdef WITH_KINEMATICS
  public:
    PerigeeParameters6D convert(
      const Vector7D &, const Charge,
      const Point3D & referencePoint = Point3D(0.,0.,0.)) const;
    PerigeeCovariance6D convert(
      const Covariance7D & error, const Vector7D & state, const Charge charge,
      const Point3D & referencePoint = Point3D(0.,0.,0.)) const;
  #endif
};

} // namespace rave
