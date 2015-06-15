#include "RaveBase/RaveInterface/rave/Vector7D.h"
#include "RaveBase/Converters/interface/CmsToRaveObjects.h"

#include <iomanip>

using namespace std;

namespace rave
{

Vector7D::Vector7D()
    : Vector6D(), them ( 0. )
{}

Vector7D::Vector7D ( float x, float y, float z,
                     float px, float py, float pz,
                     float m ) :
    Vector6D ( x, y, z, px, py, pz ), them ( m )
{}

Vector7D::Vector7D ( const Point3D & position, const Vector3D & momentum,
                     float m ) :
    Vector6D ( position, momentum ), them ( m )
{}

float Vector7D::m() const
{
  return them;
}

float Vector7D::energy() const
{
  return sqrt( momentum().mag2() + them*them );
}

rave::Vector4D Vector7D::p4() const
{
  return rave::Vector4D ( momentum(), them );
}

void Vector7D::add ( const Vector4D & other )
{
  addMomentum ( other.p3() );
  them=them+other.m();
}

bool Vector7D::equal ( const Vector7D & other ) const
{
  return ( momentum() == other.momentum() ) &&
         ( position() == other.position() ) &&
         ( m() == other.m() );
}

bool operator== ( const rave::Vector7D & lhs, const rave::Vector7D & rhs )
{
  return lhs.equal ( rhs );
}

} // namespace rave
