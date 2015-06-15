//
// C++ Interface: PerigeeParameters6D
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _RavePerigeeParameters6D_H_
#define _RavePerigeeParameters6D_H_

#include <rave/PerigeeParameters5D.h>

#include <ostream>

namespace rave {

/**
 * @class PerigeeParameters6D perigee parameter with invariant mass
 *
 * Extends the usual 5-parameter perigee perameter set for the invariant mass.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class RaveDllExport PerigeeParameters6D :
    public PerigeeParameters5D,
      boost::equality_comparable< PerigeeParameters6D >
{
  public:
    PerigeeParameters6D(float rho, float theta, float phip, float epsilon,
                        float zp, float mass);
    PerigeeParameters6D(const PerigeeParameters5D &, float mass);
    PerigeeParameters6D();

    ~PerigeeParameters6D();

    float mass() const; //!< returns the invariant mass
    float energy() const; //!< Calculates the full energy of an uncharged particle

    bool equal(const PerigeeParameters6D& other) const;

  protected:
    CheckedFloat theMass;

};

RaveDllExport bool operator== (const rave::PerigeeParameters6D & lhs, const rave::PerigeeParameters6D & rhs);

} // namespace rave

#endif
