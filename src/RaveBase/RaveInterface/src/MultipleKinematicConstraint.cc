#include "rave/MultipleKinematicConstraint.h"

#include "boost/cast.hpp"

namespace rave
{

MultipleKinematicConstraint::MultipleKinematicConstraint (
  const BasicMultipleKinematicConstraint & o ) :
    KinematicConstraint ( *(o.clone()) )
{}

void rave::MultipleKinematicConstraint::addConstraint(
    rave::KinematicConstraint constraint )
{
  const BasicMultipleKinematicConstraint* bmkcs = 
      dynamic_cast< const BasicMultipleKinematicConstraint* >( &data() );
  bmkcs->addConstraint( constraint );
  return;
}

}
