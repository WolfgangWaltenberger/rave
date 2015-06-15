#include "rave/KinematicParticle.h"

namespace rave
{

KinematicParticle::KinematicParticle( const BasicKinematicParticle & o ) :
    Base( o.clone() )
{}

KinematicParticle::KinematicParticle() :
    Base ( new BasicKinematicParticle () )
{}

const rave::Vector7D & KinematicParticle::fullstate() const
{
  return data().fullstate();
}

const rave::Vector4D & KinematicParticle::p4() const
{
  return data().fullstate().p4();
}

const rave::Covariance7D & KinematicParticle::fullerror() const
{
  return data().fullerror();
}

const rave::Vector6D & KinematicParticle::state() const
{
  return data().state();
}

const rave::Covariance6D & KinematicParticle::error() const
{
  return data().error();
}

Charge KinematicParticle::charge() const
{
  return data().charge();
}

const PerigeeParameters5D & rave::KinematicParticle::perigeeParameters() const
{
  return data().perigeeParameters();
}

const PerigeeCovariance5D & rave::KinematicParticle::perigeeCovariance() const
{
  return data().perigeeCovariance();
}

const PerigeeParameters6D & rave::KinematicParticle::fullPerigeeParameters() const
{
  return data().fullPerigeeParameters();
}

const PerigeeCovariance6D & rave::KinematicParticle::fullPerigeeCovariance() const
{
  return data().fullPerigeeCovariance();
}

float KinematicParticle::chi2() const
{
  return data().chi2();
}

float KinematicParticle::ndof() const
{
  return data().ndof();
}

boost::shared_ptr< rave::MagneticField > KinematicParticle::magneticField() const
{
  return data().magneticField();
}

boost::shared_ptr< rave::KinematicConstraint > KinematicParticle::lastConstraint() const
{
  return data().lastConstraint();
}

int KinematicParticle::id() const
{
  return data().id();
}

boost::any rave::KinematicParticle::link() const
{
  return data().link();
}

std::string rave::KinematicParticle::tag() const
{
  return data().tag();
}

bool rave::KinematicParticle::isValid() const
{
  return data().isValid();
}

void rave::KinematicParticle::unlink ()
{
  sharedData().unlink();
}

void rave::KinematicParticle::setInternal ( const boost::any & a )
{
  sharedData().setInternal ( a );
}

bool rave::KinematicParticle::operator<( const rave::KinematicParticle & o ) const
{
  return ( id() < o.id() );
}

}
