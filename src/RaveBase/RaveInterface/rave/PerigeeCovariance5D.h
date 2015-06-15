#ifndef _RavePerigeeCovariance5D_H_
#define _RavePerigeeCovariance5D_H_

#include <rave/PerigeeCovariance3D.h>

namespace rave {

/**
 * @class PerigeeCovariance5D stores covariance for perigee track parameters
 *
 * This data class holds error and covariance information for a corresponding
 * PerigeeParameters5D structure.
 *
 * The five parameters in the referred class are called rho, theta, phip,
 * epsilon and zp. To keep the argument names short here, they are abbreviated by
 * their first character here.
 */

class RaveDllExport PerigeeCovariance5D :
      public PerigeeCovariance3D
{
  public:
    PerigeeCovariance5D(float drr, float drt, float drp, float dre,
                        float drz, float dtt, float dtp, float dte,
                        float dtz, float dpp, float dpe, float dpz,
                        float dee, float dez, float dzz);
    PerigeeCovariance5D(const PerigeeCovariance3D &,
                        float dre, float drz,
                        float dte, float dtz,
                        float dpe, float dpz,
                        float dee, float dez, float dzz);
    PerigeeCovariance5D();

    float dre() const;
    float drz() const;
    float dte() const;
    float dtz() const;
    float dpe() const;
    float dpz() const;
    float dee() const;
    float dez() const;
    float dzz() const;

  private:
    CheckedFloat thedre;
    CheckedFloat thedrz;
    CheckedFloat thedte;
    CheckedFloat thedtz;
    CheckedFloat thedpe;
    CheckedFloat thedpz;
    CheckedFloat thedee;
    CheckedFloat thedez;
    CheckedFloat thedzz;
};

} // namespace rave

#endif
