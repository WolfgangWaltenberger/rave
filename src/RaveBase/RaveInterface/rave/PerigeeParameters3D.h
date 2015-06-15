#ifndef _RavePerigeeParameters3D_H_
#define _RavePerigeeParameters3D_H_

#include "rave/CheckedFloat.h"
#include "boost/operators.hpp"
#include <cmath>

namespace rave {

/**
 * @class PerigeeMomentum3D stores momentum-parameters only
 *
 * This data class manages a three-dimensional momentum vector in perigee
 * representation.
 *
 * The choice of parameters matches the convention used in the CMSSW framework.
 * (see P.Billoir, S.Qian, "Fast vertex fitting...", NIM A311 (1992) 139-150)
 */

class RaveDllExport PerigeeParameters3D :
    boost::equality_comparable< PerigeeParameters3D >
{
  public:
    PerigeeParameters3D(float rho, float theta, float phip);
    PerigeeParameters3D();

    float rho() const;
    float theta() const;
    float phip() const;

    bool equal(const PerigeeParameters3D & other) const;

  protected:
    CheckedFloat theRho;
    CheckedFloat theTheta;
    CheckedFloat thePhiP;
};

RaveDllExport bool operator== (const rave::PerigeeParameters3D & lhs,
                               const rave::PerigeeParameters3D & rhs);

} // namespace rave

#endif

