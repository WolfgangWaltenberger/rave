#ifndef Geom_SimpleCylinderBounds_H
#define Geom_SimpleCylinderBounds_H


#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometrySurface/interface/LocalError.h"
#include "DataFormats/GeometrySurface/interface/Bounds.h"
#include <cmath>
#include <algorithm>

/** Cylinder bounds. The cylinder axis coincides with the Z axis.
 *  The bounds limit the length at constant Z, and allow finithe thickness.
 *  The cylinder bound in this way looks like a pipe cut 
 *  perpendicularily to it's axis.
 */

class SimpleCylinderBounds : public Bounds {
public:

  SimpleCylinderBounds( float rmin, float rmax, float zmin, float zmax) : 
    theRmin(rmin), theRmax(rmax), theZmin(zmin), theZmax(zmax) {
    if ( theRmin > theRmax) std::swap( theRmin, theRmax);
    if ( theZmin > theZmax) std::swap( theZmin, theZmax);
  }

  // currently used by tracker geometry
  virtual float length()    const { return theZmax - theZmin;}
  virtual float width()     const { return 2*theRmax;}
  virtual float thickness() const { return theRmax-theRmin;}

  virtual bool inside( const Local3DPoint& p) const {
    return p.z()    > theZmin && p.z()    < theZmax &&
           p.perp() > theRmin && p.perp() < theRmax;
  }
    
  virtual bool inside( const Local3DPoint& p, const LocalError& err,float scale) const {

    SimpleCylinderBounds tmp( theRmin, theRmax,
			      theZmin - sqrt(err.yy())*scale,
			      theZmax + sqrt(err.yy())*scale);
    
    return tmp.inside(p);
  }

  virtual bool inside( const Local2DPoint& p, const LocalError& err) const {
    return Bounds::inside(p,err);
  }

  virtual Bounds* clone() const { 
    return new SimpleCylinderBounds(*this);
  }
private:
  float theRmin;
  float theRmax;
  float theZmin;
  float theZmax;
};

#endif // Geom_SimpleCylinderBounds_H
