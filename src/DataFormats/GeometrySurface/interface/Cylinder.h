#ifndef _Geom_Cylinder_H_
#define _Geom_Cylinder_H_

#include "DataFormats/GeometrySurface/interface/Surface.h"

class Cylinder : public virtual Surface {
public:

  Cylinder( const PositionType& pos, const RotationType& rot, Scalar radius) :
    Surface( pos, rot), theRadius(radius) {}

  Cylinder( const PositionType& pos, const RotationType& rot, Scalar radius,
	 MediumProperties* mp) : 
    Surface( pos, rot, mp), theRadius(radius) {}

// extension of Surface interface for cylinder

  Scalar radius() const {return theRadius;}

// implementation of Surface interface    

  using Surface::side;
  virtual Side side( const LocalPoint& p, Scalar toler) const;

  /// tangent plane to surface from global point
  virtual ReferenceCountingPointer<TangentPlane> tangentPlane (const GlobalPoint&) const;
  /// tangent plane to surface from local point
  virtual ReferenceCountingPointer<TangentPlane> tangentPlane (const LocalPoint&) const;

private:

  Scalar theRadius;

};

#endif
