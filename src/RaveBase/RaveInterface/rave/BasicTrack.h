#ifndef _RaveBasicTrack_H_
#define _RaveBasicTrack_H_

#include <rave/Vector6D.h>
#include <rave/Covariance6D.h>
#include <rave/Charge.h>
#include <rave/ReferenceCounted.h>
#include <rave/CopyUsingNew.h>
#include <rave/ProxyBase.h>
#include <rave/AbstractTrack.h>
#include <rave/AbstractPerigeeAccess.h>
#include <string>
#include <vector>
#include <utility>

namespace rave
{
/** \class BasicRaveBasicTrack defines the input to the rave library.
 * Fixme, it's more something like a RaveTrajectoryState,
 * than a BasicRaveBasicTrack.
 */

class RaveDllExport BasicTrack : 
	public RaveReferenceCounted, 
	public AbstractTrack,
        public AbstractPerigeeAccess
{
  public:
    typedef RaveProxyBase< BasicTrack, CopyUsingNew<BasicTrack> > Proxy;

    /**
     *  Create a BasicRaveBasicTrack from a 6-dimensional euclidean trajectory state
     *  plus covariance matrix, plus charge, plus an optional
     *  pointer to the "original track", whatever that means
     */
    BasicTrack( const Vector6D &, const Covariance6D &, Charge,
           float chi2, float ndof,
           void * originaltrack = 0, std::string tag="" );
    BasicTrack( int id, const Vector6D &, const Covariance6D &, Charge,
           float chi2, float ndof,
           void * originaltrack = 0, std::string tag="" );
    BasicTrack ( const std::vector < std::pair < float, rave::BasicTrack > > & );
    BasicTrack ( int id, const std::vector < std::pair < float, rave::BasicTrack > > & );

    void setComponents ( const std::vector < std::pair < float, rave::BasicTrack > > & );

    BasicTrack();

    Charge charge() const;
    const Vector6D & state() const;
    const Vector3D & momentum() const;
    const Point3D & position() const;
    const Covariance6D & error() const;
    void * originalObject() const;
    std::string tag() const;
    int id() const;
    float chi2() const;
    float ndof() const;

    const PerigeeParameters5D & perigeeParameters() const;
    const PerigeeCovariance5D & perigeeCovariance() const;

    bool isValid() const;

    BasicTrack * clone () const;

    std::vector < std::pair < float, rave::BasicTrack > > components() const;

    bool operator< ( const rave::BasicTrack & ) const;

  private:
    void createMomPos();
    
    Vector6D theState;
    Vector3D theMomentum;
    Point3D thePosition;
    Covariance6D theError;
    Charge theCharge;
    void * thePointer;
    std::string theTag;
    int theId;
    bool theIsValid;
    float theChi2;
    float theNdof;
    std::vector < std::pair < float, BasicTrack > > theComponents;

  private:
    void calculateCachedPerigeeRepresentation() const;
    mutable bool hasCachedPerigeeRepresentation;
    mutable PerigeeParameters5D cachedPerigeeParameters;
    mutable PerigeeCovariance5D cachedPerigeeCovariance;
};

}

#endif
