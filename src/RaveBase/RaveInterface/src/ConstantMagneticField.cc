#include "RaveBase/RaveInterface/rave/ConstantMagneticField.h"
#include <iostream>

rave::MagneticField * rave::ConstantMagneticField::copy() const
{
  return new rave::ConstantMagneticField ( *this );
}

rave::Vector3D rave::ConstantMagneticField::inTesla ( const rave::Point3D & ) const
{
  // std::cout << "[rave::ConstantMagneticField] ::inTesla called." << std::endl;
  return rave::Vector3D ( x_, y_, z_ );
}

rave::ConstantMagneticField::ConstantMagneticField() : x_(0.), y_(0.), z_(0.) {}
rave::ConstantMagneticField::ConstantMagneticField( float x, float y, float z ) : 
  x_(x), y_(y), z_(z) {}
rave::ConstantMagneticField::ConstantMagneticField( float z ) : 
  x_(0.), y_(0.), z_(z) {}
