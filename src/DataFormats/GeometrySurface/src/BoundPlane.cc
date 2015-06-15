////#include \"Utilities/Configuration/interface/Architecture.h"

#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include "DataFormats/GeometrySurface/interface/TangentPlane.h"
#include "DataFormats/GeometrySurface/interface/OpenBounds.h"
//#include "Utilities/GenUtil/interface/ReferenceCountingPointer.h"

BoundPlane::BoundPlane( const PositionType& pos, 
			const RotationType& rot) :
    Plane( pos, rot), BoundSurface(pos, rot, OpenBounds()), Surface( pos,rot) {}

