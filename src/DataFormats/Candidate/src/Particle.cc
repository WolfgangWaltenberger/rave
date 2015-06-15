#include "DataFormats/Candidate/interface/Particle.h"
#include <cmath>

using namespace std;

reco::Particle::LorentzVector::LorentzVector ( double x, double y, double z, double t ) :
  x_ ( x ), y_ ( y ) , z_ ( z ), t_ ( t )
{}

double reco::Particle::LorentzVector::x() const
{
  return x_;
}
double reco::Particle::LorentzVector::y() const
{
  return y_;
}
double reco::Particle::LorentzVector::z() const
{
  return z_;
}
double reco::Particle::LorentzVector::t() const
{
  return t_;
}

reco::Particle::Particle ( double px, double py, double pz, double E ) :
  px_ ( px ), py_ ( py ) , pz_ ( pz ), E_ ( E )
{}

reco::Particle::Particle ( char Charge, LorentzVector p ) :
  px_ ( p.x() ), py_ ( p.y() ), pz_ ( p.z() ), E_ ( p.t() )
{}

reco::Particle::Particle() 
{}

double reco::Particle::px() const
{
  return px_;
}

double reco::Particle::py() const
{
  return py_;
}

double reco::Particle::pz() const
{
  return pz_;
}

double reco::Particle::eta() const
{
  double rho = pt();
  if (rho > 0) {
    double z_scaled ( pz_ /rho );
    return log(z_scaled+sqrt(z_scaled*z_scaled+1)); // faster 
  } else if ( pz_==0) {
    return 0;
  } else if ( pz_>0) {
    return pz_ + 22756. ;
  }  else {
    return pz_ - 22756. ;
  }
}

double reco::Particle::pt() const
{
  return sqrt ( px_ * px_ + py_ * py_ );
}

double reco::Particle::p() const
{
  return sqrt ( px_ * px_ + py_ * py_ + pz_ * pz_ );
}

