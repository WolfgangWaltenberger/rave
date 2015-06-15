#include "rave/KinematicTree.h"

namespace rave
{

KinematicTree::KinematicTree (
  const BasicKinematicTree & o ) :
    Base ( o.clone() )
{}

KinematicTree::KinematicTree() :
    Base ( new BasicKinematicTree() )
{}

bool KinematicTree::isEmpty() const
{
  return data().isEmpty();
}

bool KinematicTree::isConsistent() const
{
  return data().isConsistent();
}

bool KinematicTree::isValid() const
{
  return data().isValid();
}

std::vector< rave::KinematicParticle > KinematicTree::finalStateParticles() const
{
  return data().finalStateParticles();
}

rave::KinematicParticle KinematicTree::topParticle() const
{
  return data().topParticle();
}

rave::KinematicVertex KinematicTree::currentDecayVertex() const
{
  return data().currentDecayVertex();
}

rave::KinematicVertex KinematicTree::currentProductionVertex() const
{
  return data().currentProductionVertex();
}

rave::KinematicParticle KinematicTree::currentParticle() const
{
  return data().currentParticle();
}

std::pair< bool, rave::KinematicParticle > KinematicTree::motherParticle() const
{
  return data().motherParticle();
}

std::vector< rave::KinematicParticle > KinematicTree::daughterParticles() const
{
  return data().daughterParticles();
}

void KinematicTree::movePointerToTheTop() const
{
  return data().movePointerToTheTop();
}

bool KinematicTree::movePointerToTheMother() const
{
  return data().movePointerToTheMother();
}

bool KinematicTree::movePointerToTheFirstChild() const
{
  return data().movePointerToTheFirstChild();
}

bool KinematicTree::movePointerToTheNextChild() const
{
  return data().movePointerToTheNextChild();
}

bool KinematicTree::findParticle ( rave::KinematicParticle part ) const
{
  return data().findParticle ( part );
}

bool KinematicTree::findDecayVertex ( rave::KinematicVertex vert ) const
{
  return data().findDecayVertex ( vert );
}

void KinematicTree::unlink()
{
  sharedData().unlink();
}

}
