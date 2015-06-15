#ifndef _VacuumPropagator_H_
#define _VacuumPropagator_H_

#include <rave/Propagator.h>

namespace rave
{

/** 
 * @brief A simple VacuumPropagator class
 */

class RaveDllExport VacuumPropagator : 
	public Propagator
{
  public:
    VacuumPropagator();
    virtual Propagator * copy() const;
    virtual rave::Track closestTo ( const rave::Track &,
        const rave::Point3D &, bool transverse ) const;
    virtual std::pair < rave::Track, double > to ( const rave::Track & orig,
                          const ravesurf::Plane & ) const;
    virtual std::pair < rave::Track, double > to ( const rave::Track & orig,
                          const ravesurf::Cylinder & ) const;

    virtual ~VacuumPropagator();
};

}

#endif
