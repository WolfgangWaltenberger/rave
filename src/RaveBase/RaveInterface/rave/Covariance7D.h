//
// C++ Interface: RaveCovariance7D
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVECOVARIANCE7D_H
#define RAVECOVARIANCE7D_H

#include <rave/Covariance6D.h>

#include <ostream>

namespace rave
{

/**
 * rave::Covariance7D holds the error data for kinematical fit data objects,
 * namely rave::KinematicParticle and family. It corresponds *to the internal
 * KinematicParametersError.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class RaveDllExport Covariance7D :
      public Covariance6D,
      boost::equality_comparable<Covariance7D>
{

  public:
    Covariance7D ( float dxx, float dxy, float dxz,
                   float dyy, float dyz, float dzz,
                   float dxpx, float dxpy, float dxpz,
                   float dypx, float dypy, float dypz,
                   float dzpx, float dzpy, float dzpz,
                   float dpxpx, float dpxpy, float dpxpz,
                   float dpypy, float dpypz, float dpzpz,
                   float dxm, float dym, float dzm,
                   float dpxm, float dpym, float dpzm,
                   float dmm );

    Covariance7D ( Covariance6D space_error,
                   float dxm, float dym, float dzm,
                   float dpxm, float dpym, float dpzm,
                   float dmm );

    Covariance7D();

    virtual ~Covariance7D() {};

    float dxm() const;
    float dym() const;
    float dzm() const;
    float dpxm() const;
    float dpym() const;
    float dpzm() const;
    float dmm() const;

    bool equal ( const Covariance7D & other ) const;

  private:
    CheckedFloat thedxm;
    CheckedFloat thedym;
    CheckedFloat thedzm;
    CheckedFloat thedpxm;
    CheckedFloat thedpym;
    CheckedFloat thedpzm;
    CheckedFloat thedmm;
};

RaveDllExport bool operator== ( const rave::Covariance7D & lhs, const rave::Covariance7D & rhs );

} // namespace rave

#endif
