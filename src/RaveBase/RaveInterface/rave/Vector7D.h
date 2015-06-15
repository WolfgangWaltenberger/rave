//
// C++ Interface: RaveVector7D
//
// Description:
//
// The Vector7D objects hold kinematic particle data. They extend the
// Vector6D from the geometrical fit to the necessary 7th dimension.
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEVECTOR7D_H
#define RAVEVECTOR7D_H

#include <rave/Vector6D.h>
#include <rave/Vector4D.h>

#include <ostream>

namespace rave
{

/**
 * @class Vector7D adds the mass/energy information to a Vector6D.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 *
 * Lengths are given in cm, momenta in GeV/c.
 * Masses in GeV.
 */

class RaveDllExport Vector7D :
    public Vector6D,
    boost::equality_comparable<Vector7D>
{

  public:
    Vector7D();

    Vector7D ( float x, float y, float z,
               float px, float py, float pz,
               float m );

    Vector7D ( const Point3D & position,
               const Vector3D & momentum,
               float m );

    virtual ~Vector7D() {};

    float m() const;
    float energy() const; //!< Calculates the full energy of this particle
    Vector4D p4() const;

    /// add a momentum 4-vector to our momentum four-vector
    void add ( const Vector4D & p4 );


    bool equal(const Vector7D & other) const;

  private:
    CheckedFloat them;

};

RaveDllExport bool operator==(const rave::Vector7D & lhs, const rave::Vector7D & rhs);

} // namespace rave

#endif
