#ifndef Geom_ConeBuilder_H
#define Geom_ConeBuilder_H

#include "DataFormats/GeometrySurface/interface/BoundCone.h"
//#include "Utilities/GenUtil/interface/ReferenceCountingPointer.h"
#include "DataFormats/GeometrySurface/interface/OpenBounds.h"

class Bounds;

/** Recommended way to instantiate BuondCones.
 *  The BoundCone is reference counted, so only ReferenceCountingPointer
 *  should be used to point to it, and it should only be created on the heap.
 *  The ConeBuilder enforces these constraints.
 *  It will eventually become the only way to instantiate cones.
 */

class ConeBuilder {
public:

  typedef Surface::PositionType                  PositionType;
  typedef Surface::RotationType                  RotationType;
  typedef ReferenceCountingPointer<BoundCone>    ReturnType;

  /** Builds a cone with origin at pos and with rotation matrix rot
   */
  ReturnType cone( const PositionType& pos, const RotationType& rot) const {
    return ReturnType( new BoundCone( pos, rot, OpenBounds()));
  }

  /** Same as above, with bounds. The bounds are cloned, and a new copy
   *  is used in the cone, so you don't have to create them with "new",
   *  and if you do, don't forget to delete them.
   */
  ReturnType cone( const PositionType& pos, const RotationType& rot, 
		       const Bounds& bounds) const {
    return ReturnType( new BoundCone( pos, rot, bounds));
  }

};

#endif
