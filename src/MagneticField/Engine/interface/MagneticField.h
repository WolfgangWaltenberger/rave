#ifndef MagneticField_MagneticField_h
#define MagneticField_MagneticField_h

#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

class MagneticField
{
public:
  MagneticField() {;};
  virtual ~MagneticField() {;};
  virtual GlobalVector inTesla ( const GlobalPoint& ) const = 0;

  GlobalVector inInverseGeV(const GlobalPoint& gp) const {                                      
    return inTesla(gp) * 2.99792458e-3;             
  }
};

#endif
