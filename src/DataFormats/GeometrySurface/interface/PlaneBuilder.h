#ifndef Geom_PlaneBuilder_H
#define Geom_PlaneBuilder_H

#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
//#include "Utilities/GenUtil/interface/ReferenceCountingPointer.h"

class Bounds;

/** Recommended way to instantiate BuondPlanes.
 *  The BoundPlane is reference counted, so only ReferenceCountingPointer
 *  should be used to point to it, and it should only be created on the heap.
 *  The PlaneBuilder enforces these constraints.
 *  It will eventually become the only way to instantiate planes.
 */

class PlaneBuilder {
public:

  typedef Surface::PositionType                  PositionType;
  typedef Surface::RotationType                  RotationType;
  typedef ReferenceCountingPointer<BoundPlane>   ReturnType;

  /** Builds a plane with origin at pos and with rotation matrix rot
   */
  ReturnType plane( const PositionType& pos, const RotationType& rot) const {
    return ReturnType( new BoundPlane( pos, rot));
  }

  /** Same as above, with bounds. The bounds are cloned, and a new 
   *  copy is used in the plane, so you don't have to create them with "new",
   *  and if you do, don't forget to delete them.
   */
  ReturnType plane( const PositionType& pos, const RotationType& rot, 
		    const Bounds& bounds) const {
    return ReturnType( new BoundPlane( pos, rot, bounds));
  }

};

#endif
