#include "rave/KinematicConstraint.h"

namespace rave
{

KinematicConstraint::KinematicConstraint() :
    Base ( new BasicKinematicConstraint() )
{
}

KinematicConstraint::KinematicConstraint (
  const BasicKinematicConstraint & o ) :
    Base ( o.clone() )
{
}

}
