//
// C++ Interface: PerigeeCovariance6D
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEPERIGEECOVARIANCE6D_H
#define RAVEPERIGEECOVARIANCE6D_H

#include <rave/PerigeeCovariance5D.h>

#include <ostream>

namespace rave {

/**
   * @class PerigeeCovariance6D stores covariance data associated with a PerigeeParameters6D
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class PerigeeCovariance6D :
      public PerigeeCovariance5D
{
  public:
    PerigeeCovariance6D(
      float drr, float drt, float drp, float dre, float drz, float drm,
      float dtt, float dtp, float dte, float dtz, float dtm, float dpp,
      float dpe, float dpz, float dpm, float dee, float dez, float dem,
      float dzz, float dzm, float dmm);
    PerigeeCovariance6D(
      const PerigeeCovariance5D &,
      float drm, float dtm, float dpm, float dem, float dzm, float dmm);
    PerigeeCovariance6D();

    float drm() const;
    float dtm() const;
    float dpm() const;
    float dem() const;
    float dzm() const;
    float dmm() const;

  private:
    CheckedFloat thedrm;
    CheckedFloat thedtm;
    CheckedFloat thedpm;
    CheckedFloat thedem;
    CheckedFloat thedzm;
    CheckedFloat thedmm;
};

} // namespace rave

#endif
