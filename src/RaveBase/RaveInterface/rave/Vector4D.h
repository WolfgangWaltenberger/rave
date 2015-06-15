//
// C++ Interface: Vector4D
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEVECTOR4D_H
#define RAVEVECTOR4D_H

#include <rave/Vector3D.h>

namespace rave
{

/**
        @author Fabian Moser <fabiamos@gmail.com>
*/
class RaveDllExport Vector4D :
      public rave::Vector3D,
      boost::equality_comparable<Vector4D>,
      boost::additive< Vector4D >
{
  public:
    Vector4D( float x, float y, float z, float m );
    Vector4D( rave::Vector3D v, float m );
    Vector4D();

    float m() const;
    float energy() const;

    bool equal( const rave::Vector4D & ) const;
    /// add other to this
    void add(const rave::Vector4D & other);

    /// subtract other from this
    void subtract(const rave::Vector4D & other);

    /// explicitly ask for momentum 3-vector 
    Vector3D p3() const;

  private:
    CheckedFloat them;

};

RaveDllExport bool operator==(const rave::Vector4D & lhs, const rave::Vector4D & rhs);
RaveDllExport rave::Vector4D & operator+=( rave::Vector4D & lhs, const rave::Vector4D & rhs);
RaveDllExport rave::Vector4D & operator-=( rave::Vector4D & lhs, const rave::Vector4D & rhs);

} // namespace rave

#endif
