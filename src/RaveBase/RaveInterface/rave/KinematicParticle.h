#ifndef RAVEKINEMATICPARTICLE_H
#define RAVEKINEMATICPARTICLE_H

#include <rave/BasicKinematicParticle.h>
#include <ostream>

namespace rave
{

/**
 * @brief Holds the parameters and error information of one particle.
 * @ingroup Rave
 * @author Fabian Moser <fabian@hephy.oeaw.ac.at>
 *
 * This class manages the seven parameters (position, momentum and mass)
 * of one reconstructed particle together with the corresponding
 * 7x7 covariance matrix and additional statistical information.
 *
 * It is used as input data class for the kinematic fitting through the
 * methods of the KinematicTreeFactory class. But it is also an output
 * data class being embedded in the KinematicTree instances returned by
 * those fitting methods.
 *
 * Because of this dual purpose, this class cannot be instantiated itself
 * but rather serves as common base class for the 
 * TransientTrackKinematicParticle which is the actual input data class
 * and for the VirtualKinematicParticle which can only be instantiated
 * by the KinematicTreeFactory and represents the particles generated
 * during the kinematic fitting.
 */
class RaveDllExport KinematicParticle :
      public BasicKinematicParticle::Proxy,
      public boost::less_than_comparable< KinematicParticle >
{
  protected:
    typedef BasicKinematicParticle::Proxy Base;

    friend class BasicKinematicParticle;
    friend class BasicTransientTrackKinematicParticle;
    friend class BasicVirtualKinematicParticle;
    friend class BasicKinematicTree;

    /**
     * This constructor is protected to completely hide the 
     * BasicKinematicParticle internals from the Rave user.
     */
    KinematicParticle( const rave::BasicKinematicParticle & );
    
    void setInternal ( const boost::any & a );
  public:
    /**
     * Using this default constructor renders the resulting instance invalid 
     * by means of the isValid() method.
     */
    KinematicParticle();

    /**
     * The fullstate represents the 7-dimensional tuple including position,
     * momentum and mass of the particle.
     * @return 7-dimensional state vector
     */
    const rave::Vector7D & fullstate() const;

    /**
     * The fullstate represents the 4-dimensional tuple including 
     * momentum and mass of the particle.
     * @return 4-dimensional state vector
     */
    const rave::Vector4D & p4() const;

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

    /**
     * @return The charge associated with this particle
     */
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
     * @return The last constraint applied to this particle.
     */
    boost::shared_ptr< rave::KinematicConstraint > lastConstraint() const;

    int id() const;
    boost::any link() const;
    std::string tag() const;

    /**
     * This method is used to recognize if the instance has been created using
     * the default constructor which would make it invalid. Valid instances
     * need to be of the type TransientTrackKinematicParticle and be created 
     * using one of its constructors or of the type VirtualKinematicParticle
     * and be created by the KinematicTreeFactory.
     */
    bool isValid() const;

    bool operator< ( const rave::KinematicParticle & ) const;

    /**
     *  removes all links to other particles, needed for
     *  destruction.
     */
    void unlink ();
};

}

#endif
