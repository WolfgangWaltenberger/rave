#ifndef _RaveAbstractTrack_H_
#define _RaveAbstractTrack_H_

#include <rave/Charge.h>
#include <rave/Covariance6D.h>
#include <rave/Vector6D.h>

namespace rave 
{
class RaveDllExport AbstractTrack 
{
  /**
   *  Abstract base class for all vertices.
   */
  public:  
    virtual Charge charge()  const = 0;
    virtual const Covariance6D & error() const = 0;
    virtual const Vector6D & state() const = 0;
    virtual ~AbstractTrack() {};
    virtual int id() const = 0;
};

}

#endif
