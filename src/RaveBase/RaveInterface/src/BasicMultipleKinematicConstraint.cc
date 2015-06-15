#include "rave/MultipleKinematicConstraint.h"

#include "RecoVertex/KinematicFitPrimitives/interface/MultipleKinematicConstraint.h"

#include "boost/cast.hpp"

template<class T, class U>
boost::shared_ptr<T>
polymorphic_pointer_cast(boost::shared_ptr<U> const & r)
{
  return boost::shared_ptr<T>(r, boost::polymorphic_cast<T*>(r.get()));
}

namespace rave
{

namespace
{
typedef boost::shared_ptr< ::MultipleKinematicConstraint > InternalMultipleKinematicContraint;
}

void BasicMultipleKinematicConstraint::addConstraint ( KinematicConstraint constraint ) const
{
  ::KinematicConstraint* internalConstraint =
    boost::any_cast< boost::shared_ptr< ::KinematicConstraint > > ( constraint.data().getInternal() ).get();
#if 1
  polymorphic_pointer_cast< ::MultipleKinematicConstraint > (
    boost::any_cast< boost::shared_ptr< ::KinematicConstraint > > ( getInternal() ) )->addConstraint (
      internalConstraint );
#else
  boost::shared_polymorphic_cast< ::MultipleKinematicConstraint > (
    boost::any_cast< boost::shared_ptr< ::KinematicConstraint > > ( getInternal() ) )->addConstraint (
      internalConstraint );
#endif

  constraints.push_back ( constraint );
}

BasicMultipleKinematicConstraint::BasicMultipleKinematicConstraint (
  boost::any constraint ) :
    BasicKinematicConstraint ( constraint )
{
  BOOST_ASSERT ( typeid ( boost::shared_ptr< ::MultipleKinematicConstraint > ) == constraint.type() );
}

BasicMultipleKinematicConstraint::~BasicMultipleKinematicConstraint()
{}

BasicMultipleKinematicConstraint * BasicMultipleKinematicConstraint::clone() const
{
  return new BasicMultipleKinematicConstraint( *this );
}

}
