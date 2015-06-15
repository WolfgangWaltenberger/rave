#ifndef _RaveMagneticField_H_
#define _RaveMagneticField_H_

#include <rave/Point3D.h>
#include <rave/Vector3D.h>

/*
#include "DataFormats/GeometryVector/interface/GlobalVector.h>
#include "DataFormats/GeometryVector/interface/GlobalPoint.h>
*/

namespace rave
{

/** \class RaveMagneticField makes sure we can
 * access the "exterior" magnetic field from within rave.
 * The standard unit here is Tesla.
 */

class RaveDllExport MagneticField
{
  public:
    virtual Vector3D inTesla ( const Point3D & ) const = 0;
    virtual MagneticField * copy() const = 0;
    virtual ~MagneticField() {};
};

}

#endif
