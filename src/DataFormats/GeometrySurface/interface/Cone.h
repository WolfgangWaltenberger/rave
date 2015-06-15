#ifndef Geom_Cone_H
#define Geom_Cone_H

#include "DataFormats/GeometrySurface/interface/Surface.h"

class Cone : public virtual Surface {
public:

  Cone( const PositionType& pos, const RotationType& rot, 
	const PositionType& vert, Geom::Theta<Scalar> angle) :
    Surface( pos, rot), theVertex(vert), theAngle(angle) {}

// extension of Surface interface for cylinder

  GlobalPoint vertex() const {return theVertex;}

  Geom::Theta<float> openingAngle() const {return theAngle;}

// implementation of Surface interface    

  virtual Side side( const LocalPoint& p, Scalar tolerance) const {return side( toGlobal(p), tolerance);}
  virtual Side side( const GlobalPoint& p, Scalar tolerance) const;

  /// tangent plane to surface from global point
  virtual ReferenceCountingPointer<TangentPlane> tangentPlane (const GlobalPoint&) const;
  /// tangent plane to surface from local point
  virtual ReferenceCountingPointer<TangentPlane> tangentPlane (const LocalPoint&) const;

private:

  GlobalPoint             theVertex;
  Geom::Theta<Scalar>     theAngle;

};

#endif
