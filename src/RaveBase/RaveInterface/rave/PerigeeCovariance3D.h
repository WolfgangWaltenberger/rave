#ifndef _RavePerigeeCovariance3D_H_
#define _RavePerigeeCovariance3D_H_

#include "rave/CheckedFloat.h"

namespace rave {

/**
 * @class PerigeeCovariance3D stores covariance for perigee momentum
 *
 * This data class holds error and covariance information for a corresponding
 * PerigeeParameters3D structure.
 *
 * The three parameters in the referred class are called rho, theta and
 * phip. To keep the argument names short here, they are abbreviated by their
 * first characters here.
 */

class RaveDllExport PerigeeCovariance3D
{
  public:
    PerigeeCovariance3D(float drr, float drt, float drp,
                        float dtt, float dtp, float dpp);
    PerigeeCovariance3D();

    float drr() const; /// d rho * d rho
    float drt() const; /// d rho * d theta
    float drp() const; /// d rho * d phip
    float dtt() const; /// d theta * d theta
    float dtp() const; /// d theta * d phip
    float dpp() const; /// d phip * dphip

  private:
    CheckedFloat thedrr;
    CheckedFloat thedrt;
    CheckedFloat thedrp;
    CheckedFloat thedtt;
    CheckedFloat thedtp;
    CheckedFloat thedpp;
};

} // namespace rave

#endif
