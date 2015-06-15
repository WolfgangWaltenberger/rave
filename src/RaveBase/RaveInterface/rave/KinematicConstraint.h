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
#ifndef RAVEKINEMATICCONSTRAINT_H
#define RAVEKINEMATICCONSTRAINT_H

#include <rave/BasicKinematicConstraint.h>

namespace rave
{

/**
 @author Fabian Moser <fabiamos@gmail.com>
*/
class RaveDllExport KinematicConstraint :
      public BasicKinematicConstraint::Proxy
{
    typedef BasicKinematicConstraint::Proxy Base;
    friend class KinematicConstraintBuilder;

  public:
    KinematicConstraint();

  protected:
    KinematicConstraint ( const rave::BasicKinematicConstraint & );

};

}

#endif
