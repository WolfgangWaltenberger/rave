#include "RaveBase/Converters/interface/MagneticFieldWrapper.h"
#include "RaveBase/RaveInterface/rave/Vector3D.h"

using namespace std;

GlobalVector MagneticFieldWrapper::inTesla ( const GlobalPoint & gp ) const
{
  if ( !theField )
  {
    cout << "[MagneticFieldWrapper] Arrg! No field registered!" << endl;

  }
  rave::Vector3D rv = theField->inTesla( rave::Point3D( gp.x(), gp.y(), gp.z() ) );
  return GlobalVector( rv.x(), rv.y(), rv.z() );
  // return theField->inTesla ( o );
}

MagneticFieldWrapper::MagneticFieldWrapper( const MagneticFieldWrapper & o ) :
  theField ( o.theField->copy() )
{ }

MagneticFieldWrapper::MagneticFieldWrapper ( const rave::MagneticField & r ) :
  theField ( r.copy() )
{}

MagneticFieldWrapper::~MagneticFieldWrapper()
{
  delete theField;
}
