#ifndef _RavePoint3D_H_
#define _RavePoint3D_H_

#include "rave/CheckedFloat.h"
#include "boost/operators.hpp"
#include <cmath>

namespace rave
{

/**
 * @class Point3D a point in 3d space.
 *
 * Lengths are given in cm in rave, momenta in GeV/c.
 * Masses and energies in GeV.
 */

class RaveDllExport Point3D :
    boost::equality_comparable< Point3D >,
    boost::additive< Point3D >,
    boost::multiplicative< Point3D, double >
{
  public:

    Point3D( float x, float y, float z );
    Point3D();

    float x() const;
    float y() const;
    float z() const;

    float mag() const;
    float mag2() const;

    float perp() const;
    float perp2() const;

    bool equal(const Point3D & other) const;

    /// add other to this
    void add(const Point3D & other);

    /// subtract other from this
    void subtract(const Point3D & other);
    
    void multiply ( double scale );

  protected:
    CheckedFloat thex;
    CheckedFloat they;
    CheckedFloat thez;
};

RaveDllExport bool operator==(const rave::Point3D & lhs, const rave::Point3D & rhs);
RaveDllExport rave::Point3D & operator+=( rave::Point3D & lhs, const rave::Point3D & rhs);
RaveDllExport rave::Point3D & operator*=( rave::Point3D & lhs, double scale );
RaveDllExport rave::Point3D & operator-=( rave::Point3D & lhs, const rave::Point3D & rhs);

} // namespace rave


#endif

