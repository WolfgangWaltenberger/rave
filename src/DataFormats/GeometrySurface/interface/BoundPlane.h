#ifndef Geom_BoundPlane_H
#define Geom_BoundPlane_H

#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "DataFormats/GeometrySurface/interface/BoundSurface.h"

/** A plane in 3D space, with bounds.
 *  Do not instantiate directly, use PlaneBuilder instead.
 */

class BoundPlane : public Plane, public BoundSurface {
public:

  // BoundPlane()  {}

  virtual ~BoundPlane() {}

  BoundPlane( const PositionType& pos, 
	      const RotationType& rot, 
	      const Bounds* bounds) :
    Surface( pos,rot), Plane( pos, rot), BoundSurface(pos, rot, bounds) {}

  BoundPlane( const PositionType& pos, 
	      const RotationType& rot, 
	      const Bounds& bounds) :
    Surface( pos,rot), Plane( pos, rot), BoundSurface(pos, rot, bounds) {}

  /// Bound Plane with unlimited bounds
  BoundPlane( const PositionType& pos, 
	      const RotationType& rot);

};

#endif // Geom_BoundPlane_H
