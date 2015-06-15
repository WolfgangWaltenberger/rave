#include "RaveBase/RaveInterface/rave/BasicKinematicTree.h"

#include <rave/TransientTrackKinematicParticle.h>
#include <rave/VirtualKinematicParticle.h>

#include "RaveBase/RaveEngine/interface/RaveId.h"

#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicTree.h"
#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/VirtualKinematicParticle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "boost/bind.hpp"

namespace rave
{

bool BasicKinematicTree::isEmpty() const
{
  if (!theIsValid) return true;
  return boost::any_cast< ::RefCountedKinematicTree > ( theTree )->isEmpty();
}

bool BasicKinematicTree::isConsistent() const
{
  if (!theIsValid) return false;
  return boost::any_cast< ::RefCountedKinematicTree > ( theTree )->isConsistent();
}

bool BasicKinematicTree::isValid() const
{
  return theIsValid;
}

std::vector< rave::KinematicParticle > BasicKinematicTree::finalStateParticles() const
{
  if (!theIsValid) return std::vector< rave::KinematicParticle > ();
  std::vector< rave::KinematicParticle > externalResult;
  ::RefCountedKinematicTree internalTree = 
      boost::any_cast< ::RefCountedKinematicTree >( theTree );
  internalTree->movePointerToTheTop();
  std::vector< ::RefCountedKinematicParticle > internalResult =
      internalTree->finalStateParticles();
  std::transform (
    internalResult.begin(),
    internalResult.end(),
    std::back_inserter ( externalResult ),
    boost::bind ( &BasicKinematicTree::lookupParticle, this, _1 ) );
  return externalResult;
}

rave::KinematicParticle BasicKinematicTree::topParticle() const
{
  if (!theIsValid) return rave::KinematicParticle();
  return lookupParticle ( boost::any_cast< ::RefCountedKinematicTree > ( theTree )->topParticle() );
}

rave::KinematicVertex BasicKinematicTree::currentDecayVertex() const
{
  if (!theIsValid) return rave::KinematicVertex();
  return lookupVertex ( boost::any_cast< ::RefCountedKinematicTree > ( theTree )->currentDecayVertex() );
}

rave::KinematicVertex BasicKinematicTree::currentProductionVertex() const
{
  if (!theIsValid) return rave::KinematicVertex();
  return lookupVertex ( boost::any_cast< ::RefCountedKinematicTree > ( theTree )->currentProductionVertex() );
}

rave::KinematicParticle BasicKinematicTree::currentParticle() const
{
  if (!theIsValid) return rave::KinematicParticle();
  return lookupParticle ( boost::any_cast< ::RefCountedKinematicTree > ( theTree )->currentParticle() );
}

pair< bool, rave::KinematicParticle > BasicKinematicTree::motherParticle() const
{
  if (!theIsValid) return pair< bool, rave::KinematicParticle > ( false, rave::KinematicParticle() );
  pair< bool, ::RefCountedKinematicParticle > internalResult =
    boost::any_cast< ::RefCountedKinematicTree > ( theTree )->motherParticle();
  return make_pair ( internalResult.first, lookupParticle ( internalResult.second ) );
}

std::vector< rave::KinematicParticle > BasicKinematicTree::daughterParticles() const
{
  if (!theIsValid) return std::vector< rave::KinematicParticle >();
  std::vector< rave::KinematicParticle > externalResult;

  std::vector< ::RefCountedKinematicParticle > internalResult =
    boost::any_cast< ::RefCountedKinematicTree > ( theTree )->daughterParticles();

  std::transform (
    internalResult.begin(),
    internalResult.end(),
    std::back_inserter ( externalResult ),
    boost::bind ( &BasicKinematicTree::lookupParticle, this, _1 ) );

  return externalResult;
}

void BasicKinematicTree::movePointerToTheTop() const
{
  return boost::any_cast< ::RefCountedKinematicTree > ( theTree )->movePointerToTheTop();
}

bool BasicKinematicTree::movePointerToTheMother() const
{
  return boost::any_cast< ::RefCountedKinematicTree > ( theTree )->movePointerToTheMother();
}

bool BasicKinematicTree::movePointerToTheFirstChild() const
{
  return boost::any_cast< ::RefCountedKinematicTree > ( theTree )->movePointerToTheFirstChild();
}

bool BasicKinematicTree::movePointerToTheNextChild() const
{
  return boost::any_cast< ::RefCountedKinematicTree > ( theTree )->movePointerToTheNextChild();
}

bool BasicKinematicTree::findParticle ( rave::KinematicParticle part ) const
{
  return boost::any_cast< ::RefCountedKinematicTree > ( theTree )->findParticle (
           boost::any_cast< ::RefCountedKinematicParticle > ( part.data().getInternal() ) );
}

bool BasicKinematicTree::findDecayVertex ( rave::KinematicVertex vert ) const
{
  return boost::any_cast< ::RefCountedKinematicTree > ( theTree )->findDecayVertex (
           boost::any_cast< ::RefCountedKinematicVertex > ( vert.data().getInternal() ) );
}

BasicKinematicTree * BasicKinematicTree::clone() const
{
  return new BasicKinematicTree ( *this );
}

BasicKinematicTree::BasicKinematicTree (
  boost::any internalTree,
  std::vector< rave::KinematicParticle > particles, boost::any link,
  std::string tag ) : theTree ( internalTree ), 
    theId ( RaveId::uniqueId() ), theLink ( link ),
    theTag ( tag ), theIsValid ( true )
{
  BOOST_ASSERT( typeid( ::RefCountedKinematicTree ) == internalTree.type() );
  LogDebug( "BasicKinematicTree" ) << "Assigning " << particles.size() << " particles to tree";
  initializeMaps( particles );
}

BasicKinematicTree::BasicKinematicTree (
  int id, boost::any internalTree,
  std::vector< rave::KinematicParticle > particles, boost::any link,
  std::string tag ) :
    theTree ( internalTree ), theId ( id ), theLink ( link ), theTag ( tag ),
    theIsValid ( true )
{
  BOOST_ASSERT ( typeid ( ::RefCountedKinematicTree ) == internalTree.type() );

  if ( particles.size() > 0 ) initializeMaps ( particles );
}

BasicKinematicTree::BasicKinematicTree() :
    theIsValid ( false )
{}

boost::any BasicKinematicTree::getInternal() const
{
  return theTree;
}

void BasicKinematicTree::unlink()
{
  for ( map < void *, rave::KinematicParticle >::iterator i=particleMap.begin(); 
        i!=particleMap.end() ; ++i )
  {
    i->second.unlink();
  }

  for ( map < void *, rave::KinematicVertex >::iterator i=vertexMap.begin(); 
        i!=vertexMap.end() ; ++i )
  {
    i->second.unlink();
  }

  theTree=boost::any();
  theLink=boost::any();
}

BasicKinematicTree::~BasicKinematicTree()
{
  unlink();
  particleMap.clear();
  vertexMap.clear();
}


bool BasicKinematicTree::compareParticles (
  void * op, rave::KinematicParticle ip )
{
  return op == boost::any_cast< ::RefCountedKinematicParticle > ( ip.data().getInternal() ).get();
}

void BasicKinematicTree::initializeMaps(
    std::vector< rave::KinematicParticle > inputParticles )
{
  // Delete all cached instances of interface objects
  particleMap.clear();
  vertexMap.clear();

  // Get the "roots" of the decay chain
  ::RefCountedKinematicTree internalTree = 
      boost::any_cast< ::RefCountedKinematicTree >( theTree );
  internalTree->movePointerToTheTop();
  std::vector< ::RefCountedKinematicParticle > rootParticles =
      internalTree->finalStateParticles();

  // For each of the root particles go back the history to their origin and
  // compare with the core of the input particles
  for ( std::vector< ::RefCountedKinematicParticle >::iterator p = 
            rootParticles.begin();
        p != rootParticles.end(); p++ )
  {
    RefCountedKinematicParticle op = *p;

    // Unroll history
    //while ( op->previousParticle() ) op = op->previousParticle();

    // Search the input particle field for a corresponding interface object
    std::vector< rave::KinematicParticle >::iterator it =
      std::find_if( inputParticles.begin(),
                    inputParticles.end(),
                    boost::bind( &BasicKinematicTree::compareParticles, this, 
                                 op.get(), _1 ) );

    // If the search was successful, add the association to the particle map
    if ( it != inputParticles.end() )
    {
      // Update the backlink
      it->sharedData().setInternal( op );
      particleMap[ p->get() ] = ( *it );
    }
  }
}

/**
 * @todo throw if the type of a new particle was neither virtual nor transient-track
 */
KinematicParticle BasicKinematicTree::lookupParticle ( boost::any internalParticle ) const
{
  BOOST_ASSERT ( typeid ( ::RefCountedKinematicParticle ) == internalParticle.type() );

  // Look up an existing cached interface object
  map< void*, rave::KinematicParticle >::iterator it =
    particleMap.find ( boost::any_cast< ::RefCountedKinematicParticle > ( internalParticle ).get() );

  if ( it != particleMap.end() )
  {
    // If successful, return the found object
    return ( ( *it ).second );
  }
  else
  {
    KinematicParticle newParticle;

    // If not successful, check the type of the internal particle and create
    // a corresponding interface particle
    if ( dynamic_cast< ::VirtualKinematicParticle * > (
           boost::any_cast< ::RefCountedKinematicParticle > ( internalParticle ).get() ) )
    {
      // This is a virtual particle
      newParticle = KinematicParticle ( BasicVirtualKinematicParticle ( internalParticle ) );
    }
    else
      if ( dynamic_cast< ::TransientTrackKinematicParticle* > (
             boost::any_cast< ::RefCountedKinematicParticle > ( internalParticle ).get() ) )
      {
        // This is a transient track based particle
        newParticle = KinematicParticle ( BasicTransientTrackKinematicParticle ( internalParticle ) );
      }

    particleMap.insert (
      make_pair ( boost::any_cast< ::RefCountedKinematicParticle > ( internalParticle ).get(), newParticle ) );

    return newParticle;
  }
}

rave::KinematicVertex BasicKinematicTree::lookupVertex ( boost::any internalVertex ) const
{
  BOOST_ASSERT ( typeid ( ::RefCountedKinematicVertex ) == internalVertex.type() );

  // Look up an existing cached interface object
  map< void*, rave::KinematicVertex >::iterator it =
    vertexMap.find ( boost::any_cast< ::RefCountedKinematicVertex > ( internalVertex ).get() );

  if ( it != vertexMap.end() )
  {
    // If successful, return the found object
    return ( ( *it ).second );
  }
  else
  {
    rave::KinematicVertex newVertex;

    // Create a corresponding interface vertex
    newVertex = rave::KinematicVertex ( BasicKinematicVertex ( internalVertex ) );

    vertexMap.insert (
      make_pair ( boost::any_cast< ::RefCountedKinematicVertex > ( internalVertex ).get(), newVertex ) );

    return newVertex;
  }
}


}
