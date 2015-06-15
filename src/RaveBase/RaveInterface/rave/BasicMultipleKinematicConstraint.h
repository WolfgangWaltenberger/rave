//
// C++ Interface: MultipleKinematicConstraint
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEBASICMULTIPLEKINEMATICCONSTRAINT_H
#define RAVEBASICMULTIPLEKINEMATICCONSTRAINT_H

#include <rave/KinematicConstraint.h>

#include <vector>

namespace rave
{

/**
 * @class MultipleKinematicConstraint provides the ability to combine
 * (multiply) a number of kinematic constraints.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
*/
class RaveDllExport BasicMultipleKinematicConstraint :
    public BasicKinematicConstraint
{
  public:
    typedef RaveProxyBase< BasicMultipleKinematicConstraint, 
                           CopyUsingNew<BasicMultipleKinematicConstraint> > Proxy;

    friend class KinematicConstraintBuilder;

    virtual ~BasicMultipleKinematicConstraint();

    /**
     * This adds an existing constraint to the bundle.
     * @param constraint The existing constraint to be added
     *
     * @exception std::bad_cast Thrown when this instance does not contain a kinematics::MultipleKinematicConstraint
     */
    void addConstraint ( rave::KinematicConstraint constraint ) const;

    BasicMultipleKinematicConstraint * clone () const;

  protected:
    BasicMultipleKinematicConstraint ( boost::any constraint );

  private:
    /** This is for mere lifetime management of the contained constraints,
     * because the kinematics::MultipleKinematicConstraint uses a vector<*>
     */
    mutable std::vector< KinematicConstraint > constraints;
};

}

#endif
