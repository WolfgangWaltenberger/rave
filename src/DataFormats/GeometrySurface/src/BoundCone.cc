////#include \"Utilities/Configuration/interface/Architecture.h"

/*
 *  See header file for a description of this class.
 *
 *  $Date: 2005/07/15 18:30:07 $
 *  $Revision: 1.1 $
 */

#include "DataFormats/GeometrySurface/interface/BoundCone.h"
#include "DataFormats/GeometrySurface/interface/SimpleConeBounds.h"

#include "DataFormats/GeometrySurface/interface/TangentPlane.h"

BoundCone::BoundCone( const PositionType& pos, const RotationType& rot, 
		      const Bounds* bounds) :
    Surface( pos,rot),
    Cone( pos, rot, 
	  dynamic_cast<const SimpleConeBounds&>(*bounds).vertex(),
	  dynamic_cast<const SimpleConeBounds&>(*bounds).openingAngle()),
    BoundSurface(pos, rot, bounds) {}
	  
BoundCone::BoundCone( const PositionType& pos, const RotationType& rot, 
		      const Bounds& bounds) :
    Surface( pos,rot),
    Cone( pos, rot, 
	  dynamic_cast<const SimpleConeBounds&>(bounds).vertex(),
	  dynamic_cast<const SimpleConeBounds&>(bounds).openingAngle()),
    BoundSurface(pos, rot, bounds) {}
