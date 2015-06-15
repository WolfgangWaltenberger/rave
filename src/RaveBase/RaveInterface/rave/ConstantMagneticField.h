#ifndef _RaveInterfaceConstantMagneticField_H_
#define _RaveInterfaceConstantMagneticField_H_

#include <rave/MagneticField.h>

namespace rave {

/** 
 * @brief A simple constant magnetic field that ships with Rave.
 * @ingroup Rave
 */

class RaveDllExport ConstantMagneticField : 
      public MagneticField
{
  public:
    virtual Vector3D inTesla ( const Point3D & ) const;
    virtual MagneticField * copy() const;
    ConstantMagneticField();
    ConstantMagneticField( float x, float y, float z );
    ConstantMagneticField( float z );

    virtual ~ConstantMagneticField() {};
  private:
    float x_;
    float y_;
    float z_;
};

} // namespace rave

#endif
