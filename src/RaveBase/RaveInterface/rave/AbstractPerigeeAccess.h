#ifndef _RaveAbstractPerigeeAccess_H_
#define _RaveAbstractPerigeeAccess_H_

#include <rave/PerigeeParameters5D.h>
#include <rave/PerigeeCovariance5D.h>

namespace rave
{

/**
 * @class AbstractPerigeeAccess is a perigee parameter interface
 *
 * This is an interface class for classes providing access to perigee 
 * parameters
 */
class RaveDllExport AbstractPerigeeAccess
{
  public:
    virtual const PerigeeParameters5D & perigeeParameters() const = 0;
    virtual const PerigeeCovariance5D & perigeeCovariance() const = 0;
    virtual ~AbstractPerigeeAccess() {};
};

} // namespace rave

#endif
