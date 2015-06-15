#ifndef Geom_Bounds_H
#define Geom_Bounds_H


#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"

class LocalError;

/** Abstract base class for Bounds.
 *  Bounds provide a general way to specify the form of a concrete 
 *  surface. For example, a BoundPlane with TrapezoidalPlaneBounds
 *  has a trapezoidal shape.
 */ 

class Bounds {
public:
  virtual ~Bounds() {}

  // currently used by tracker geometry
  virtual float length() const = 0;
  virtual float width() const = 0;
  virtual float thickness() const = 0;

  /** Width at half length. Useful for e.g. pitch definition.
   */
  virtual float widthAtHalfLength() const {return width();}

  // basic bounds function
  virtual bool inside( const Local3DPoint&) const = 0;
    
  // For propagation with uncertainties - one has to know by how
  // much one missed a surface
  // virtual Local2DVector<float> howFar( const Local2DPoint&) = 0;
  //
  // Or maybe a better way of determining if a surface is "touched"
  // by a trajectory is to have a method

  virtual bool inside( const Local3DPoint&, const LocalError&, float scale=1.f) const = 0;

  virtual bool inside( const Local2DPoint& p, const LocalError& err, 
		       float scale=1.f) const {
    return inside( Local3DPoint(p.x(), p.y(), 0), err, scale);
  }

  virtual Bounds* clone() const = 0;

};

#endif
