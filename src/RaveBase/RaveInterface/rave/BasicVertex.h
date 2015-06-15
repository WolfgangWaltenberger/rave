#ifndef _RaveBasicVertex_H_
#define _RaveBasicVertex_H_

#include <rave/Track.h>
#include <rave/Point3D.h>
#include <rave/Covariance3D.h>
#include <rave/Covariance33D.h>
#include <rave/CopyUsingNew.h>
#include <rave/ProxyBase.h>
#include <rave/AbstractVertex.h>
#include <utility>
#include <vector>
#include <map>

namespace rave 
{

class RaveDllExport BasicVertex : 
	public RaveReferenceCounted, 
	public AbstractVertex
{
  /**
   *  The output of the rave library: a reconstructed vertex.
   *  A position in 3d space, plus its error.
   *  Along with the (refitted!!) tracks and
   *  the association probabilities of tracks belonging to this vertex.
   *  also a (fractional) number of degrees of freedom (ndf)
   *  and chi-squared.
   */
  public:  
    typedef RaveProxyBase< BasicVertex, CopyUsingNew<BasicVertex> > Proxy;
    const Point3D & position() const;
    const Covariance3D & error() const;
    const Covariance33D & trackToVertexCovariance( int id ) const;
    const Covariance3D & trackToTrackCovariance( int id1, int id2 ) const;
    const std::vector < std::pair < float, Track > > & weightedTracks() const;
    std::vector < Track > tracks() const;
    std::vector < Track > refittedTracks() const;
    Track refittedTrack ( const Track & orig ) const;
    const std::vector < std::pair < float, Track > > & weightedRefittedTracks() const;
    float ndf() const;
    float chiSquared() const;
    int id() const;
    bool isValid() const;
    bool hasRefittedTracks() const;
    // std::vector < BasicVertex > components() const;

  public:
    BasicVertex(); // default constructor, produces invalid vertex.
    BasicVertex( const Point3D &, const Covariance3D &,
            const std::vector < std::pair < float, Track > > & tracks, float ndf, float chi2 );
    BasicVertex( const Point3D &, const Covariance3D &,
            const std::vector < std::pair < float, Track > > & tracks,
            const std::vector < std::pair < float, Track > > & fittedtracks, float ndf, float chi2 );
    BasicVertex( const Point3D &, const Covariance3D &,
            const std::vector < Track > & tracks, float ndf, float chi2 );
    BasicVertex( const Point3D &, const Covariance3D &,
            const std::vector < Track > & tracks,
            const std::vector < Track > & fittedtracks, float ndf, float chi2 );

    /// add track to vertex covariances one by one
    void addTrackToVertexCovariance ( int trackid, const Covariance33D & );
    void addTrackToTrackCovariance ( int t1, int t2, const Covariance3D & );
    
    bool hasTrackToVertexCovariance() const;

  private:
    Point3D thePoint;
    Covariance3D theError;
    std::vector < std::pair < float, Track > > theTracks;
    std::vector < std::pair < float, Track > > theFittedTracks;
    mutable std::map < int, Covariance33D > theTkToVtxCovs;
    mutable std::map < int, std::map < int, Covariance3D > > theTkToTkCovs;
    // std::vector < BasicVertex > theComponents;
    float theNdf;
    float theChi2;
    int theId;
    bool theValid;
};

}

#endif
