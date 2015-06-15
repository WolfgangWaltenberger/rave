#ifndef Geom_BoundCone_H
#define Geom_BoundCone_H

/** \class BoundCone
 *  A cone with bounds. 
 *  Do not instantiate directly, use ConeBuilder instead.
 *
 *  $Date: 2005/07/15 18:30:07 $
 *  $Revision: 1.1 $
 */

#include "DataFormats/GeometrySurface/interface/Cone.h"
#include "DataFormats/GeometrySurface/interface/BoundSurface.h"
#include "DataFormats/GeometryVector/interface/Theta.h"

class TangentPlane;
template <class T> class ReferenceCountingPointer;

class BoundCone : public Cone, public BoundSurface {
public:

  BoundCone( const PositionType& pos, const RotationType& rot, const Bounds* bounds);

  BoundCone( const PositionType& pos, const RotationType& rot, const Bounds& bounds);

};

#endif // Geom_BoundCone_H

