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
#ifndef RAVEMULTIPLEKINEMATICCONSTRAINT_H
#define RAVEMULTIPLEKINEMATICCONSTRAINT_H

#include <rave/BasicMultipleKinematicConstraint.h>

namespace rave
{

/**
 @author Fabian Moser <fabiamos@gmail.com>
*/
class RaveDllExport MultipleKinematicConstraint :
      public KinematicConstraint
{
    friend class KinematicConstraintBuilder;

  public:
    void addConstraint ( rave::KinematicConstraint constraint );

  protected:
    MultipleKinematicConstraint ( const rave::BasicMultipleKinematicConstraint & );

};

}

#endif
