#include "RaveBase/RaveInterface/rave/Track.h"
#include "RaveBase/RaveEngine/interface/RaveId.h"

using namespace rave;
using namespace std;

vector < pair < float, BasicTrack > > Track::convert ( const vector < pair < float, Track > > & o ) const
{
  vector < pair < float, BasicTrack > > ret;
  for ( vector< pair < float, Track > >::const_iterator i=o.begin(); i!=o.end() ; ++i )
  {
    ret.push_back ( pair < float, BasicTrack > ( i->first, i->second.basicTrack() ) );
  }
  return ret;
}

Track::Track( const Vector6D & s, const Covariance6D & e, Charge q,
   float chi2, float ndof, void * p, string d ) : 
     Base ( new BasicTrack ( s, e, q, chi2, ndof, p, d ) )
{}

Track::Track( int id, const Vector6D & s, const Covariance6D & e, Charge q,
   float chi2, float ndof, void * p, string d ) : 
     Base ( new BasicTrack ( id, s, e, q, chi2, ndof, p, d ) )
{}

Track::Track( const Vector6D & s, const Covariance6D & e, Charge q,
   float chi2, float ndof, int p, string d ) : 
  Base ( new BasicTrack ( s, e, q, chi2, ndof, (void *) (p), d ) )
{}

Track::Track ( const vector < pair < float, Track > > & c ) :
  Base ( new BasicTrack  ( convert ( c ) ) ),
  theComponents ( c )
{}

Track::Track ( int id, const vector < pair < float, Track > > & c ) :
  Base ( new BasicTrack  ( id, convert ( c ) ) ),
  theComponents ( c )
{}

Track::Track ( const BasicTrack & o ) : Base ( o.clone() )
{}

Track::Track () : Base ( new BasicTrack () )
{}

const rave::BasicTrack & Track::basicTrack() const
{
  return data();
}

string Track::tag() const
{
  return data().tag();
}

Charge Track::charge() const
{
  return data().charge();
}

int Track::id() const
{
  return data().id();
}

const Vector6D & Track::state() const
{
  return data().state();
}

const Vector3D & Track::momentum() const
{
  return data().momentum();
}

const Point3D & Track::position() const
{
  return data().position();
}

float Track::pt() const
{
  return this->momentum().perp();
}

const Covariance6D & Track::error() const
{
  return data().error();
}

void * Track::originalObject() const
{
  return data().originalObject();
}

vector < pair < float, Track > > Track::components() const
{
  return theComponents;
}

bool Track::isValid() const
{
  return data().isValid();
}

bool Track::operator< ( const Track & o ) const
{
    return ( id() < o.id() );
}

bool Track::operator== ( const Track & o ) const
{
  return ( id() == o.id() );
}

float Track::chi2() const
{
  return data().chi2();
}

float Track::ndof() const
{
  return data().ndof();
}

const PerigeeParameters5D & Track::perigeeParameters() const
{
  return data().perigeeParameters();
}

const PerigeeCovariance5D & Track::perigeeCovariance() const
{
  return data().perigeeCovariance();
}
