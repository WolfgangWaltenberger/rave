//
// C++ Interface: Covariance4D
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVECOVARIANCE4D_H
#define RAVECOVARIANCE4D_H

#include <rave/Covariance3D.h>

namespace rave
{

/**
        @author Fabian Moser <fabiamos@gmail.com>
*/
class RaveDllExport Covariance4D :
    public rave::Covariance3D
{
  public:
    Covariance4D ( float dxx, float dxy, float dxz, float dxm,
                   float dyy, float dyz, float dym,
                   float dzz, float dzm, float dmm );
    Covariance4D();

    float dxm() const;
    float dym() const;
    float dzm() const;
    float dmm() const;

    bool equal ( const rave::Covariance4D & other ) const;

  private:
    CheckedFloat thedxm;
    CheckedFloat thedym;
    CheckedFloat thedzm;
    CheckedFloat thedmm;

};

RaveDllExport bool operator==(const rave::Covariance4D & lhs, const rave::Covariance4D & rhs);

} // namespace rave

#endif
