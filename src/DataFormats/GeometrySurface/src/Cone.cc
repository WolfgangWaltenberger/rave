////#include \"Utilities/Configuration/interface/Architecture.h"

#include "DataFormats/GeometrySurface/interface/Cone.h"
#include "DataFormats/GeometrySurface/interface/TangentPlane.h"

#include <iostream>

ReferenceCountingPointer<TangentPlane> Cone::tangentPlane (const GlobalPoint&) const {
  // FIXME: to be implemented...
  std::cout << "*** WARNING: Cone::tangentPlane not implemented." <<std::endl;
  return ReferenceCountingPointer<TangentPlane>();
}

ReferenceCountingPointer<TangentPlane> Cone::tangentPlane (const LocalPoint&) const {
  // FIXME: to be implemented...
  std::cout << "*** WARNING: Cone::tangentPlane not implemented." <<std::endl;
  return ReferenceCountingPointer<TangentPlane>();
}

Surface::Side Cone::side( const GlobalPoint& p, Scalar tolerance) const {
  // tolerance is interpreted as max distance from cone surface.
  // FIXME: check case when vertex().z()==inf.
  GlobalPoint p1(p.x(), p.y(), p.z()-vertex().z());

  // handle the singularity of p=vertex (i.e. p1.mag() undefined)
  if (p1.mag()<tolerance) return SurfaceOrientation::onSurface;
  double delta = double(p1.theta())- double(openingAngle());
  if (fabs(delta) < tolerance/p1.mag()) return SurfaceOrientation::onSurface;
  
  if (p1.theta() < Geom::pi()/2.) {
    return (delta>0. ?  SurfaceOrientation::positiveSide : SurfaceOrientation::negativeSide);
  } else {
    return (delta>0. ?  SurfaceOrientation::negativeSide : SurfaceOrientation::positiveSide);
  }
}
