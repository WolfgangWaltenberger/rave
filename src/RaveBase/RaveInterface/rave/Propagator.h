#ifndef _RavePropagator_H_
#define _RavePropagator_H_

#include <rave/Track.h>
#include <rave/Plane.h>
#include <rave/Cylinder.h>
#include <utility>

namespace rave {

/** 
 * @brief Guarantees that all necessary propagations can be performed. 
 * @ingroup Rave
 *
 * Returns another rave::Track, albeit at a different position.
 */

class RaveDllExport Propagator
{
  public:
    virtual Propagator * copy() const = 0;
    virtual rave::Track closestTo ( const rave::Track & orig,
                          const rave::Point3D &, bool transverse ) const = 0;
    /**
     *  propagate to plane, returing new track
     *  and propagated distance 
     */
    virtual std::pair < rave::Track, double > to ( const rave::Track & orig,
                          const ravesurf::Plane & ) const = 0;
    /**
     *  propagate to cylinder, returning new track
     *  and propagated distance
     */
    virtual std::pair < rave::Track, double > to ( const rave::Track & orig,
                          const ravesurf::Cylinder & ) const = 0;

    virtual ~Propagator() {};
};

} // namespace rave

#endif
