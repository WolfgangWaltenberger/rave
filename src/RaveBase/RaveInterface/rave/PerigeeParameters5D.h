#ifndef _RavePerigeeParameters5D_H_
#define _RavePerigeeParameters5D_H_

#include <rave/PerigeeParameters3D.h>

#include "boost/operators.hpp"

namespace rave {

/**
 * @class PerigeeParameters5D stores track perigee parameters
 *
 * This data class manages a track state in his perigee representation.
 *
 * The choice of parameters matches the convention used within the CMSSW
 * framework.
 * (see P.Billoir, S.Qian, "Fast vertex fitting...", NIM A311 (1992) 139-150)
 */

class RaveDllExport PerigeeParameters5D :
    public rave::PerigeeParameters3D,
    boost::equality_comparable< PerigeeParameters5D >
{
  public:
    PerigeeParameters5D(float rho, float theta, float phip, float epsilon, float zp);
    PerigeeParameters5D(const PerigeeParameters3D &, float epsilon, float zp);
    PerigeeParameters5D();

    float epsilon() const;
    float zp() const;

    bool equal(const PerigeeParameters5D & other) const;

  protected:
    CheckedFloat theEpsilon;
    CheckedFloat theZP;
};

RaveDllExport bool operator== (const rave::PerigeeParameters5D & lhs, const rave::PerigeeParameters5D & rhs);

} // namespace rave

#endif
