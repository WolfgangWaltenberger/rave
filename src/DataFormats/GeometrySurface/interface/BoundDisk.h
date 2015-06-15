#ifndef Geom_BoundDisk_H
#define Geom_BoundDisk_H

#include "DataFormats/GeometrySurface/interface/BoundPlane.h"

/** A disk with bounds is a special plane that has an inner and outer radius. 
 */

class BoundDisk : public BoundPlane {
public:

  // BoundDisk()  {}

  virtual ~BoundDisk() {}

  BoundDisk( const PositionType& pos, 
	     const RotationType& rot, 
	     Bounds* bounds) :
    Surface(pos,rot), BoundPlane( pos, rot, bounds) {}

  BoundDisk( const PositionType& pos, 
	     const RotationType& rot, 
	     const Bounds& bounds) :
    Surface(pos,rot), BoundPlane( pos, rot, bounds) {}

  float innerRadius() const;
  float outerRadius() const;

};


#endif // Geom_BoundDisk_H
