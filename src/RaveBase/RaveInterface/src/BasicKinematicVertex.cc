#include "RaveBase/RaveInterface/rave/KinematicVertex.h"

#include "RaveBase/RaveEngine/interface/RaveId.h"
#include "RaveBase/Converters/interface/CmsToRaveObjects.h"
#include "RaveBase/Converters/interface/RaveToCmsObjects.h"

#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicVertex.h"

#include "boost/assert.hpp"
#include "boost/cast.hpp"

namespace rave
{

BasicKinematicVertex::~ BasicKinematicVertex()
{}

const rave::Point3D & BasicKinematicVertex::position() const
{
  BOOST_ASSERT (
    CmsToRaveObjects().convert (
      boost::any_cast< ::RefCountedKinematicVertex > ( theVertex )->position()
    ) ==
    thePosition );
  return thePosition;
}

const rave::Covariance3D & BasicKinematicVertex::error() const
{
  BOOST_ASSERT (
    CmsToRaveObjects().convert (
      boost::any_cast< ::RefCountedKinematicVertex > ( theVertex )->error()
    ) ==
    theError );
  return theError;
}

boost::weak_ptr< rave::BasicKinematicTree > BasicKinematicVertex::correspondingTree() const
{
  return theTree;
}

float BasicKinematicVertex::ndf() const
{
  BOOST_ASSERT (
    boost::numeric_cast<float> (
      boost::any_cast< ::RefCountedKinematicVertex > ( theVertex )->degreesOfFreedom()
    ) ==
    theNdof );
  return theNdof;
}

float BasicKinematicVertex::chiSquared() const
{
  BOOST_ASSERT (
    boost::numeric_cast<float> (
      boost::any_cast< ::RefCountedKinematicVertex > ( theVertex )->chiSquared()
    ) ==
    theChi2 );
  return theChi2;
}

int BasicKinematicVertex::id() const
{
  return theId;
}

bool BasicKinematicVertex::isValid() const
{
  return boost::any_cast< ::RefCountedKinematicVertex > ( theVertex )->vertexIsValid();
}

BasicKinematicVertex * rave::BasicKinematicVertex::clone() const
{
  return new BasicKinematicVertex ( *this );
}

BasicKinematicVertex::BasicKinematicVertex (
  boost::any vertex, boost::any link, std::string tag ) :
    thePosition ( CmsToRaveObjects().convert ( boost::any_cast< ::RefCountedKinematicVertex > ( vertex )->position() ) ),
    theError ( CmsToRaveObjects().convert ( boost::any_cast< ::RefCountedKinematicVertex > ( vertex )->error() ) ),
    theNdof ( boost::numeric_cast<float> ( boost::any_cast< ::RefCountedKinematicVertex > ( vertex )->degreesOfFreedom() ) ),
    theChi2 ( boost::numeric_cast<float> ( boost::any_cast< ::RefCountedKinematicVertex > ( vertex )->chiSquared() ) ),
    theId ( RaveId::uniqueId() ), theLink ( link ), theTag ( tag ),
    theVertex ( vertex )
{}

BasicKinematicVertex::BasicKinematicVertex (
  const rave::Point3D & position, const rave::Covariance3D & error,
  float ndf, float chi2, boost::any link, std::string tag ) :
    thePosition ( position ), theError ( error ), theNdof ( ndf ), theChi2 ( chi2 ),
    theId ( RaveId::uniqueId() ), theLink ( link ), theTag ( tag ),
    theVertex ( ::RefCountedKinematicVertex ( new ::KinematicVertex (
                  ::VertexState ( RaveToCmsObjects().convert ( position ),
                                  RaveToCmsObjects().convert ( error ) ),
                  boost::numeric_cast<float> ( chi2 ),
                  boost::numeric_cast<float> ( ndf ) ) ) )
{}

BasicKinematicVertex::BasicKinematicVertex() :
    theId ( RaveId::uniqueId() ),
    theVertex ( ::RefCountedKinematicVertex ( new ::KinematicVertex() ) )
{}

boost::any BasicKinematicVertex::getInternal() const
{
  return theVertex;
}

void BasicKinematicVertex::unlink()
{
  theLink=boost::any();
  theVertex=boost::any();
}

}
