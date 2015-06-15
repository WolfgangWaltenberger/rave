#ifndef Geom_BoundSurface_H
#define Geom_BoundSurface_H

#include <memory>
#include "DataFormats/GeometrySurface/interface/Surface.h"

#include "DataFormats/GeometrySurface/interface/Bounds.h"
//#include "Utilities/GenUtil/interface/own_ptr.h"

/** Adds Bounds to Surface. 
 *  Surfaces which differ only by 
 *  the shape of their bounds are of the same "surface" type 
 *  (e.g. Plane or Cylinder).
 */

class BoundSurface : public virtual Surface {
public:

  BoundSurface( const PositionType& pos, 
		const RotationType& rot, 
		const Bounds* bounds) :
    Surface( pos, rot), theBounds( bounds->clone()) {}

  BoundSurface( const PositionType& pos, 
		const RotationType& rot, 
		const Bounds& bounds) :
    Surface( pos, rot), theBounds( bounds.clone()) {}

    BoundSurface( const BoundSurface& iToCopy) :
    Surface( iToCopy ),
      theBounds( iToCopy.theBounds->clone() ) {}

    const BoundSurface& operator=(const BoundSurface& iRHS ) {
      theBounds = std::auto_ptr<Bounds>( iRHS.theBounds->clone() );
      return *this;
    }

  const Bounds& bounds() const { return *theBounds;}

private:
  //own_ptr<Bounds,OwnerPolicy::Clone> theBounds;
  std::auto_ptr<Bounds> theBounds;
};



#endif // Geom_BoundSurface_H
