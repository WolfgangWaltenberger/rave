#ifndef _RaveAbstractVertex_H_
#define _RaveAbstractVertex_H_

#include <rave/Point3D.h>
#include <rave/Covariance3D.h>

namespace rave 
{
class RaveDllExport AbstractVertex 
{
  /**
   *  Abstract base class for all vertices.
   */
  public:  
    virtual const Point3D & position() const = 0;
    virtual const Covariance3D & error() const = 0;
    virtual ~AbstractVertex() {};
    virtual int id() const = 0;
};

}

#endif
