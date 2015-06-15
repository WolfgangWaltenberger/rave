//
// C++ Interface: BasicKinematicParticle
//
// Description:
//
// The BasicKinematicParticle class serves as proxy base for
// rave::KinematicParticle, which is the input data structure for the
// kinematic fit part of Rave. Since this is the proxy base, the
// interesting details (TM) can be found here.
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RAVEBASICKINEMATICPARTICLE_H
#define RAVEBASICKINEMATICPARTICLE_H

#include <rave/AbstractTrack.h>
#include <rave/AbstractPerigeeAccess.h>
#include <rave/Charge.h>
#include <rave/Vector7D.h>
#include <rave/Covariance7D.h>
#include <rave/PerigeeParameters6D.h>
#include <rave/PerigeeCovariance6D.h>
#include <rave/MagneticField.h>

#include <rave/KinematicConstraint.h>

#include <rave/CopyUsingNew.h>
#include <rave/ProxyBase.h>
#include <rave/ReferenceCounted.h>

#include <string>

#include "boost/any.hpp"
#include "boost/operators.hpp"
#include "boost/shared_ptr.hpp"

namespace rave
{

class BasicKinematicTree;
class KinematicTreeFactory;
class TransientTrackKinematicParticle;

/**
 * @class BasicKinematicParticle defines the input to the rave kinematical fit library.
 *
 * This really represents an internal KinematicState object plus the necessary
 * user-provided information to "expand" it to a KinematicParticle i.e.
 * chiSquared, degreesOfFr, previousParticle and lastConstraint
 *
 * The inheritance from AbstractTrack should simplify the extraction of pure
 * geometric data.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class RaveDllExport BasicKinematicParticle :
      public RaveReferenceCounted,
      public AbstractTrack,
      public AbstractPerigeeAccess,
      public boost::less_than_comparable<BasicKinematicParticle>
{
  public:
    typedef RaveProxyBase< BasicKinematicParticle, CopyUsingNew<BasicKinematicParticle> > Proxy;

    friend class BasicKinematicTree;
    friend class KinematicTreeFactory;
    friend class TransientTrackKinematicParticle;

    /**
     * The default constructor creates an invalid particle
     */
    BasicKinematicParticle();
    BasicKinematicParticle( const BasicKinematicParticle & o );
    virtual ~BasicKinematicParticle();

    /**
     * The fullstate represents the 7-dimensional tuple including position,
     * momentum and mass of the particle.
     * @return 7-dimensional state vector
     */
    const rave::Vector7D & fullstate() const;

    /**
     * The fullerror represents the 7x7 error matrix for the state given by
     * fullstate().
     * @return 7x7 error matrix
     */
    const rave::Covariance7D & fullerror() const;

    /**
     * The state method inherited from AbstractTrack gives the 6-dimensional
     * tuple including position and energy. To obtain the mass together with
     * position and momentum, use fullstate(). This really gives the
     * full state anyway and is simply an upcast.
     * @return upcast 7-dimensional state vector
     */
    const rave::Vector6D & state() const;

    /**
     * As with state(), this gives the same as fullerror() since Covariance7D
     * is a subclass of Covariance6D.
     * @return upcast 7x7 error matrix
     */
    const rave::Covariance6D & error() const;

    Charge charge() const;

    const PerigeeParameters5D & perigeeParameters() const;
    const PerigeeCovariance5D & perigeeCovariance() const;

    const PerigeeParameters6D & fullPerigeeParameters() const;
    const PerigeeCovariance6D & fullPerigeeCovariance() const;

    float chi2() const;
    float ndof() const;

    /**
     * @return The magnetic field assigned upon creation of this particle
     */
    boost::shared_ptr< rave::MagneticField > magneticField() const;

    /**
     * @return The pointer to the kinematic tree (if any) current particle
     * belongs to 0 pointer returned in case  not any tree is built yet
     */
//    KinematicTree correspondingTree() const;

    /**
     * @return The last constraint aplied to this particle.
     */
    boost::shared_ptr< rave::KinematicConstraint > lastConstraint() const;

    int id() const;
    boost::any link() const;
    std::string tag() const;

    bool isValid() const;

    virtual BasicKinematicParticle * clone () const;
    bool operator< ( const BasicKinematicParticle & ) const;
    /** remove all links to other particles,
     *  needed for destruction. */
    void unlink();
    void setInternal ( const boost::any & );

  protected:
    /**
     * The constructor initializes the particle data structure with the given
     * values. The last constraint and the previous particle are used.
     * @param particle The particle to be interfaced by the new instance
     * @param link A backlink possibility e.g. for linking to "original" data
     * @param tag A string tag to eventually associate additional information
     */
    BasicKinematicParticle ( boost::any particle, boost::any link = boost::any(),
                             std::string tag = "" );

    /**
     * The constructor initializes the particle data structure with the given
     * values. The last constraint and the previous particle are used.
     * @param id A Rave internal and unique id
     * @param particle The particle to be interfaced by the new instance
     * @param link A backlink possibility e.g. for linking to "original" data
     * @param tag A string tag to eventually associate additional information
     */
    BasicKinematicParticle ( int id, boost::any particle,
                             boost::any link = boost::any(), std::string tag = "" );

    boost::any getInternal() const;

  private:
    Vector7D theState;
    Covariance7D theError;
    Charge theCharge;

    mutable bool hasCachedPerigeeParameters;
    mutable PerigeeParameters6D cachedPerigeeParameters;
    mutable bool hasCachedPerigeeCovariance;
    mutable PerigeeCovariance6D cachedPerigeeCovariance;

    float theNdof;
    float theChi2;

    /// Members for memory management issues
    boost::shared_ptr< rave::MagneticField > theField;
//    KinematicTree theTree;
    boost::shared_ptr< rave::KinematicConstraint > theConstraint;

    int theId;
    boost::any theLink;
    std::string theTag;

    bool theIsValid;

    mutable boost::any theParticle;

};

}

#endif
