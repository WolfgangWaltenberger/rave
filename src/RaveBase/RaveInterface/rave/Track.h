#ifndef _RaveTrack_H_
#define _RaveTrack_H_

#include <rave/Vector6D.h>
#include <rave/Covariance6D.h>
#include <rave/Charge.h>
#include <rave/BasicTrack.h>

#include <string>

namespace rave
{

/** \class RaveTrack defines the input to the rave library.
 *  The class is reference counted, so dont worry about memory management.
 */

class RaveDllExport Track : private rave::BasicTrack::Proxy
{
  typedef rave::BasicTrack::Proxy Base;
  public:

    /**
     *  Create a RaveTrack from a 6-dimensional euclidean trajectory state
     *  plus covariance matrix, plus charge, plus an optional
     *  pointer to the "original track", whatever that means.
     *
     *       Lengths are given in cm, momenta in GeV/c.
     */
    Track( const rave::Vector6D &, const rave::Covariance6D &, Charge,
           float chi2, float ndof,
           void * originaltrack = 0, std::string tag="" );
    Track( int id, const rave::Vector6D &, const rave::Covariance6D &, Charge,
           float chi2, float ndof,
           void * originaltrack = 0, std::string tag="" );
    Track( const rave::Vector6D &, const rave::Covariance6D &, rave::Charge,
           float chi2, float ndof,
           int originaltrack, std::string tag="" );

    /// Create a Track from its components.
    Track( const std::vector < std::pair < float, rave::Track > > & components );
    Track( int id, const std::vector < std::pair < float, rave::Track > > & components );

    Track ( const rave::BasicTrack & );
    Track();

    rave::Charge charge() const;
    const rave::Vector6D & state() const;
    const rave::Vector3D & momentum() const;
    const rave::Point3D & position() const;
    float pt() const;
    const rave::Covariance6D & error() const;
    void * originalObject() const;
    std::string tag() const;
    /// track id, meant to be unique for every track
    int id() const;

    /// the track id can also be set. Use with great care!
    // void setId( int );

    float chi2() const;
    float ndof() const;

    const rave::PerigeeParameters5D & perigeeParameters() const;
    const rave::PerigeeCovariance5D & perigeeCovariance() const;

    bool isValid() const;
    std::vector < std::pair < float, rave::Track > > components() const;

    bool operator< ( const rave::Track & ) const;
    bool operator== ( const rave::Track & ) const;

private:
    const rave::BasicTrack & basicTrack() const;
    std::vector < std::pair < float, BasicTrack > > convert ( 
        const std::vector < std::pair < float, Track > > & ) const;
    std::vector < std::pair < float, rave::Track > > theComponents; // we keep the components here.
};

}

#endif
