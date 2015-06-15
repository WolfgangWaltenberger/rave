#ifndef _MagneticFieldSingleton_H_
#define _MagneticFieldSingleton_H_

#include "MagneticField/Engine/interface/MagneticField.h"

class MagneticFieldSingleton : public MagneticField
{
  /** singleton that provides the magnetic field.
   */
  public:
    static MagneticFieldSingleton * Instance();
    GlobalVector inTesla ( const GlobalPoint & o ) const;
    void registry ( const MagneticField * field );
    bool hasField() const;
    const MagneticField * field() const;
  private:
    MagneticFieldSingleton();
    MagneticFieldSingleton ( const MagneticFieldSingleton & );
    const MagneticField * theField;
};

#endif
