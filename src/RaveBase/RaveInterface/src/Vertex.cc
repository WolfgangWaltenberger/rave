#include "RaveBase/RaveInterface/rave/Vertex.h"
#include "RaveBase/RaveEngine/interface/RaveId.h"
#include <iostream>

using namespace rave;
using namespace std;

Vertex::Vertex( const Point3D & p, const Covariance3D & e,
      const vector < pair < float, Track > > & tracks, float ndf, float chi2,
      const vector < pair < float, Vertex > > & components ) :
  Base ( new BasicVertex ( p, e, tracks, ndf, chi2 ) ),
  theComponents ( components )
{}

Vertex::Vertex( const Point3D & p, const Covariance3D & e,
      const vector < Track > & tracks, float ndf, float chi2,
      const vector < pair < float, Vertex > > & components ) :
  Base ( new BasicVertex ( p, e, tracks, ndf, chi2 ) ),
  theComponents ( components )
{}

Vertex::Vertex( const Point3D & p, const Covariance3D & e,
      const vector < pair < float, Track > > & tracks,
      const vector < pair < float, Track > > & ftracks, float ndf, float chi2,
      const vector < pair < float, Vertex > > & components ) :
  Base ( new BasicVertex ( p, e, tracks, ftracks, ndf, chi2 ) ),
  theComponents ( components )
{}

Vertex::Vertex( const Point3D & p, const Covariance3D & e,
      const vector < Track > & tracks, 
      const vector < Track > & ftracks, float ndf, float chi2,
      const vector < pair < float, Vertex > > & components ) :
  Base ( new BasicVertex ( p, e, tracks, ftracks, ndf, chi2 ) ),
  theComponents ( components )
{}

const Point3D & Vertex::position() const
{
  return data().position();
}

const Covariance3D & Vertex::error() const
{
  return data().error();
}

bool Vertex::hasTrackToVertexCovariance() const
{
  return data().hasTrackToVertexCovariance();
}


const rave::Covariance33D & Vertex::trackToVertexCovariance( 
    const rave::Track & i ) const
{
  return data().trackToVertexCovariance ( i.id() );
  // VertexTrack::tkTVCovariance
  /*
  static rave::Covariance3D ret;
  return ret;
  */
}

const rave::Covariance3D & Vertex::trackToTrackCovariance(
    const rave::Track & i, const rave::Track & j ) const
{
  return data().trackToTrackCovariance ( i.id(), j.id() );
}

void Vertex::addTrackToTrackCovariance(
    const rave::Track & i, const rave::Track & j,
    const rave::Covariance3D & cov ) 
{
  sharedData().addTrackToTrackCovariance ( i.id(), j.id(), cov );
}

void Vertex::addTrackToVertexCovariance(
    const rave::Track & i, const rave::Covariance33D & cov ) 
{
  sharedData().addTrackToVertexCovariance ( i.id(), cov ); // fixme
}

int Vertex::id() const
{
  return data().id();
}


const vector < pair < float, Track > > & Vertex::weightedTracks() const
{
  return data().weightedTracks();
}

const vector < pair < float, Track > > & Vertex::weightedRefittedTracks() const
{
  return data().weightedRefittedTracks();
}

vector < Track > Vertex::tracks() const
{
  return data().tracks();
}

vector < Track > Vertex::refittedTracks() const
{
  return data().refittedTracks();
}

bool Vertex::operator< ( const Vertex & o ) const
{
    return ( id() < o.id() );
}

bool Vertex::operator== ( const Vertex & o ) const
{
  return ( id() == o.id() );
}

float Vertex::ndf() const
{
  return data().ndf();
}

float Vertex::chiSquared() const
{
  return data().chiSquared();
}

bool Vertex::isValid() const
{
  return data().isValid();
}

bool Vertex::hasRefittedTracks() const
{
  return data().hasRefittedTracks();
}

Vertex::Vertex () : Base ( new BasicVertex() )
{}

Vertex::Vertex ( const BasicVertex & o ) : Base ( new BasicVertex ( o ) )
{}

std::vector < std::pair < float, Vertex > > Vertex::components() const
{
  return theComponents;
}

rave::Track Vertex::refittedTrack ( const rave::Track & orig ) const
{
  return data().refittedTrack ( orig );
}
