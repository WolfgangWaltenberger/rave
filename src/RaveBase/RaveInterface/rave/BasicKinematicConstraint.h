//
// C++ Interface: KinematicConstraint
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEBASICKINEMATICCONSTRAINT_H
#define RAVEBASICKINEMATICCONSTRAINT_H

#include <rave/CopyUsingNew.h>
#include <rave/ProxyBase.h>
#include <rave/ReferenceCounted.h>

#include "boost/any.hpp"

namespace rave
{

class KinematicConstraintBuilder;
class BasicMultipleKinematicConstraint;
class KinematicTreeFactory;

/**
 * @class BasicKinematicConstraint is a cointainer class for one
 * ::KinematicConstraint (or its subclasses) object. It's instances
 * are constructed by the builder class rave::KinematicConstraintBuilder. The
 * user of the Rave library merely uses the pointer to instances of the class
 * as parameters for the fitting algorithms.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */
class RaveDllExport BasicKinematicConstraint :
    public RaveReferenceCounted
{
  public:
    typedef RaveProxyBase< BasicKinematicConstraint, CopyUsingNew< BasicKinematicConstraint > > Proxy;

    friend class KinematicConstraintBuilder;
    friend class BasicMultipleKinematicConstraint;
    friend class KinematicTreeFactory;

    BasicKinematicConstraint();
    virtual ~BasicKinematicConstraint();

    BasicKinematicConstraint * clone () const;

    bool isValid() const;

  protected:
    /**
     * Constructs a rave::BasicKinematicConstraint containing a
     * kinematics::KinematicConstraint. This can only be done by the corresponding
     * builder (see friendship)
     * @param constraint 
     */
    BasicKinematicConstraint( boost::any constraint );

    boost::any getInternal() const;

  private:
    /// This holds the actual constraint instance
    const boost::any theConstraint;

};

}

#endif
