#ifndef _RavePlane_H_
#define _RavePlane_H_

#include <rave/Point3D.h>
#include <rave/Vector3D.h>

namespace ravesurf {

/** 
 * @brief a class implementing a physical "detector" plane
 * @ingroup ravesurf
 *
 */

class RaveDllExport Plane
{
  public:
    Plane ( const rave::Point3D & pos, const rave::Point3D & normal );
    rave::Point3D position() const;
    rave::Vector3D normalVector() const;

  private:
    rave::Point3D pos_;
    rave::Vector3D norm_;

};

} // namespace rave

#endif
