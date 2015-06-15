#include "RaveTools/Converters/interface/MagneticFieldSingleton.h"
#include "RaveBase/RaveInterface/rave/Vector3D.h"

#include <boost/assert.hpp>

using namespace std;

GlobalVector MagneticFieldSingleton::inTesla ( const GlobalPoint & gp ) const
{
  /*
  if ( !theField )
  {
    cout << "[MagneticFieldSingleton] Arrg! No field registered!" << endl;
    exit(0);
  }
  */

  BOOST_ASSERT( hasField() );

  /*
  rave::Vector3D rv = theField->inTesla( rave::Point3D( gp.x(), gp.y(), gp.z() ) );
  return GlobalVector( rv.x(), rv.y(), rv.z() );
  */
  return theField->inTesla ( gp );
}

bool MagneticFieldSingleton::hasField() const
{
  return ( theField != 0 );
}

MagneticFieldSingleton::MagneticFieldSingleton() : theField(0)
{}

MagneticFieldSingleton::MagneticFieldSingleton( const MagneticFieldSingleton & o )
{
  cout << "[MagneticFieldSingleton] Arrgh! Fatal!" << endl;
}

MagneticFieldSingleton * MagneticFieldSingleton::Instance()
{
  static MagneticFieldSingleton singleton;
  return &singleton;
}

const MagneticField * MagneticFieldSingleton::field() const
{
  return theField;
}

void MagneticFieldSingleton::registry ( const MagneticField * field )
{
  /*
  cout << "[MagneticFieldSingleton] registering field Bz="  << flush;
  cout << field->inTesla ( GlobalPoint ( 0.,0.,0. ) ).z() << flush;
  cout << " field=" << (void *) field << ", singleton=" << (void *) this << endl;
  */
  theField = field;
}
