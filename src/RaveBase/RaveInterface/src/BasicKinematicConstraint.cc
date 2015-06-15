#include "rave/BasicKinematicConstraint.h"

#include "RecoVertex/KinematicFitPrimitives/interface/KinematicConstraint.h"

#include "boost/assert.hpp"
#include "boost/shared_ptr.hpp"

namespace rave
{

namespace
{
  typedef boost::shared_ptr< ::KinematicConstraint > InternalKinematicContraint;
}

//! This implicitly leaves the theConstraint variable
// "empty" and gives negative validity on test.
BasicKinematicConstraint::BasicKinematicConstraint()
{
}

BasicKinematicConstraint::BasicKinematicConstraint (
  boost::any constraint ) :
    theConstraint ( constraint )
{
//  BOOST_ASSERT ( typeid ( boost::shared_ptr< ::KinematicConstraint > ) == constraint.type() );
}

BasicKinematicConstraint::~BasicKinematicConstraint()
{
}
    
bool BasicKinematicConstraint::isValid() const
{
  return theConstraint.empty();
}

BasicKinematicConstraint * BasicKinematicConstraint::clone() const
{
  return new BasicKinematicConstraint ( *this );
}

boost::any BasicKinematicConstraint::getInternal() const
{
  return theConstraint;
}

}
