#ifndef Geom_DiskBuilder_H
#define Geom_DiskBuilder_H

#include "DataFormats/GeometrySurface/interface/BoundDisk.h"
//#include "Utilities/GenUtil/interface/ReferenceCountingPointer.h"

class Bounds;

/** Recommended way to instantiate BuondDisks.
 *  The BoundDisk is reference counted, so only ReferenceCountingPointer
 *  should be used to point to it, and it should only be created on the heap.
 *  The DiskBuilder enforces these constraints.
 *  It will eventually become the only way to instantiate disks.
 */

class DiskBuilder {
public:

  typedef Surface::PositionType                  PositionType;
  typedef Surface::RotationType                  RotationType;
  typedef ReferenceCountingPointer<BoundDisk>    ReturnType;

  /** Builds a disk with origin at pos and with rotation matrix rot
   */
  /* FIXME
  ReturnType disk( const PositionType& pos, const RotationType& rot) const {
    return ReturnType( new BoundDisk( pos, rot ));
  }
  */

  /** Same as above, with bounds. The bounds are cloned, and a new 
   *  copy is used in the disk, so you don't have to create them with "new",
   *  and if you do, don't forget to delete them.
   */
  ReturnType disk( const PositionType& pos, const RotationType& rot, 
		    const Bounds& bounds) const {
    return ReturnType( new BoundDisk( pos, rot, bounds));
  }

};

#endif
