#ifndef Geom_Plane_H
#define Geom_Plane_H

#include "DataFormats/GeometrySurface/interface/Surface.h"

class Plane : public virtual Surface {
public:

  Plane( const PositionType& pos, const RotationType& rot) :
    Surface( pos, rot) {}

  Plane( const PositionType& pos, const RotationType& rot, MediumProperties* mp) : 
    Surface( pos, rot, mp) {}

// extension of Surface interface for planes

  GlobalVector normalVector() const {
    return GlobalVector( rotation().zx(), rotation().zy(), rotation().zz());
  }
  /** Fast access to distance from plane for a point.
   */
  float localZ (const GlobalPoint& gp) const {
    return normalVector().dot(gp-position());
  }
  /** Fast access to component perpendicular to plane for a vector.
   */
  float localZ (const GlobalVector& gv) const {
    return normalVector().dot(gv);
  }

// implementation of Surface interface    

  virtual SurfaceOrientation::Side side( const LocalPoint& p, Scalar toler) const {
    return (std::abs(p.z())<toler) ? SurfaceOrientation::onSurface : 
	(p.z()>0 ? SurfaceOrientation::positiveSide : SurfaceOrientation::negativeSide);
  }

  virtual SurfaceOrientation::Side side( const GlobalPoint& p, Scalar toler) const {
    Scalar lz = localZ(p);
    return (std::abs(lz)<toler ? SurfaceOrientation::onSurface : 
	    (lz>0 ? SurfaceOrientation::positiveSide : SurfaceOrientation::negativeSide));
  }

  /// tangent plane to surface from global point
  virtual ReferenceCountingPointer<TangentPlane> tangentPlane (const GlobalPoint&) const;

  /// tangent plane to surface from local point
  virtual ReferenceCountingPointer<TangentPlane> tangentPlane (const LocalPoint&) const;

};

#endif
