#ifndef _MagneticFieldWrapper_H_
#define _MagneticFieldWrapper_H_

#include <rave/MagneticField.h>
#include "MagneticField/Engine/interface/MagneticField.h"

class MagneticFieldWrapper : public MagneticField
{
  /** wraps a rave magnetic field for cmssw
   */
  public:
    MagneticFieldWrapper ( const rave::MagneticField & f );
    MagneticFieldWrapper ( const MagneticFieldWrapper & o );
    ~MagneticFieldWrapper();
    GlobalVector inTesla ( const GlobalPoint & o ) const;
    
  private:
    const rave::MagneticField * theField;

};

#endif
