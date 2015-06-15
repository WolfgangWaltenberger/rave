#ifndef Geom_RectangularPlaneBounds_H
#define Geom_RectangularPlaneBounds_H


#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometrySurface/interface/LocalError.h"
#include "DataFormats/GeometrySurface/interface/Bounds.h"
#include <cmath>

/**
 *  Rectangular plane bounds.
 *  Local Coordinate system coincides with center of the box
 *  with x axis along the width.
 */
class RectangularPlaneBounds : public Bounds {
public:

  RectangularPlaneBounds( float w, float h, float t) : 
    halfWidth(w), halfLength(h), halfThickness(t) {}

  // currently used by tracker geometry
  virtual float length()    const { return 2*halfLength;}
  virtual float width()     const { return 2*halfWidth;}
  virtual float thickness() const { return 2*halfThickness;}

  // basic bounds function
  virtual bool inside( const Local2DPoint& p) const {
    return fabs(p.x()) <= halfWidth && fabs(p.y()) <= halfLength;
  }

  virtual bool inside( const Local3DPoint& p) const {
    return fabs(p.x()) <= halfWidth && 
           fabs(p.y()) <= halfLength &&
           fabs(p.z()) <= halfThickness;
  }

  virtual bool inside( const Local3DPoint& p, const LocalError& err,
		       float scale) const {
    RectangularPlaneBounds tmp( halfWidth + sqrt(err.xx())*scale,
				halfLength + sqrt(err.yy())*scale,
				halfThickness);
    return tmp.inside(p);
  }
    
  virtual bool inside( const Local2DPoint& p, const LocalError& err, float scale) const {
    return Bounds::inside(p,err,scale);
  }

  virtual Bounds* clone() const { 
    return new RectangularPlaneBounds(*this);
  }


private:
  float halfWidth;
  float halfLength;
  float halfThickness;

};

#endif
