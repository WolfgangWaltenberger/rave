#ifndef Geom_CylinderBuilder_H
#define Geom_CylinderBuilder_H

#include "DataFormats/GeometrySurface/interface/BoundCylinder.h"
//#include "Utilities/GenUtil/interface/ReferenceCountingPointer.h"
#include "DataFormats/GeometrySurface/interface/OpenBounds.h"

class Bounds;

/** Recommended way to instantiate BuondCylinders.
 *  The BoundCylinder is reference counted, so only ReferenceCountingPointer
 *  should be used to point to it, and it should only be created on the heap.
 *  The CylinderBuilder enforces these constraints.
 *  It will eventually become the only way to instantiate cylinders.
 */

class CylinderBuilder {
public:

  typedef Surface::PositionType                  PositionType;
  typedef Surface::RotationType                  RotationType;
  typedef ReferenceCountingPointer<BoundCylinder>   ReturnType;

  /** Builds a cylinder with origin at pos and with rotation matrix rot
   */
  ReturnType cylinder( const PositionType& pos, const RotationType& rot) const {
    return ReturnType( new BoundCylinder( pos, rot, OpenBounds()));
  }

  /** Same as above, with bounds. The bounds are cloned, and a new copy
   *  is used in the cylinder, so you don't have to create them with "new",
   *  and if you do, don't forget to delete them.
   */
  ReturnType cylinder( const PositionType& pos, const RotationType& rot, 
		       const Bounds& bounds) const {
    return ReturnType( new BoundCylinder( pos, rot, bounds));
  }

};

#endif
