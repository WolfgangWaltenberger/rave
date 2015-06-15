#ifndef Geom_BoundCylinder_H
#define Geom_BoundCylinder_H

#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "DataFormats/GeometrySurface/interface/BoundSurface.h"


/** A cylinder with bounds. 
 *  Do not instantiate directly, use CylinderBuilder instead.
 */

class BoundCylinder : public Cylinder, public BoundSurface {
public:

  BoundCylinder( const PositionType& pos, const RotationType& rot, Scalar radius, 
		 const Bounds& bounds) :
      Surface( pos,rot), Cylinder( pos, rot, radius), BoundSurface(pos, rot, bounds) {}

  BoundCylinder( const PositionType& pos, const RotationType& rot, Scalar radius,
		 MediumProperties* mp, const Bounds& bounds) : 
      Surface( pos,rot), Cylinder( pos, rot, radius, mp), BoundSurface(pos, rot, bounds) {}

/// Obsolete constructor, radius should be given explicitly
  BoundCylinder( const PositionType& pos, 
		 const RotationType& rot, 
		 const Bounds* bounds);

/// Obsolete constructor, radius should be given explicitly
  BoundCylinder( const PositionType& pos, 
		 const RotationType& rot, 
		 const Bounds& bounds);

};

#endif // Geom_BoundCylinder_H
