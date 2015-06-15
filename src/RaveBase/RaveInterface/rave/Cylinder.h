#ifndef _RaveCylinder_H_
#define _RaveCylinder_H_

#include <rave/Point3D.h>
#include <rave/Vector3D.h>

namespace ravesurf {

/** 
 * @brief a class implementing a cylindric "detector" plane
 * @ingroup ravesurf
 *
 */

class RaveDllExport Cylinder
{
  public:
    Cylinder ( const rave::Point3D & pos, 
        const rave::Point3D & axis, float radius );
    rave::Point3D position() const;
    rave::Vector3D axis() const;
    float radius() const;

  private:
    rave::Point3D pos_;
    rave::Vector3D norm_;
    float radius_;

};

} // namespace rave

#endif
