#ifndef _RaveInterface_VertexFactory_H_
#define _RaveInterface_VertexFactory_H_

#include <rave/Track.h>
#include <rave/Vertex.h>
#include <rave/MagneticField.h>
#include <rave/Propagator.h>
#include <rave/Ellipsoid3D.h>
#include <rave/Vector3D.h>
#include <rave/Point3D.h>
#include <rave/VacuumPropagator.h>
#include <rave/ConstantMagneticField.h>
#include <vector>
#include <map>
#include <string>


namespace rave {

class VertexReconstructor;

/** 
 * @brief The central vertex fitting class which takes tracks and produces
 *        vertices according to a specified method.
 * @ingroup Rave
 *
 * The class that is central to using the Rave library. It builds the
 * RaveVertices from a set of RaveTracks.
 *
 * @param method A string that describes which algorithms should be 
 *               employed and which parameter settings should be chosen.
 *               The default is chosen such that something "sensible" is done 
 *               in most use cases.
 */
class RaveDllExport VertexFactory 
{
  public:
    VertexFactory ( const rave::MagneticField & m = rave::ConstantMagneticField(),
              const rave::Propagator & p = rave::VacuumPropagator(),
              const std::string & method = "default",
              int verbosity=1 );
    
    VertexFactory ( const rave::MagneticField &,
              const rave::Propagator &,
              const rave::Ellipsoid3D & beamspot,
              const std::string & method = "default",
              int verbosity=1 );

    VertexFactory ( const rave::VertexFactory & o );
    virtual ~VertexFactory();
    VertexFactory operator= ( const rave::VertexFactory & o );

    /** Reconstruct vertices, using the default reconstruction method
     * \paramname use_beamspot defines, if the beamspot information
     * is taken into account.
     */
    std::vector < rave::Vertex > create ( const std::vector < rave::Track > &, bool use_beamspot=false ) const;
    
    /** Reconstruct vertices, using the default reconstruction method
     * \paramname use_beamspot defines, if the beamspot information
     * is taken into account.
     * \paramname seed allows the user to supply a seed around which
     * the tracks will be linearized. Warning: this is currently 
     * ignored.
     */
    std::vector < rave::Vertex > create ( const std::vector < rave::Track > &, 
        const rave::Point3D & seed, bool use_beamspot=false ) const;


    /** Reconstruct vertices, using the default reconstruction method
     * \paramname use_beamspot defines, if the beamspot information
     * is taken into account
     * \paramname ghost_track defines, if the jetaxis information
     * is taken into account.
     */
    std::vector < rave::Vertex > create ( const std::vector < rave::Track > &, 
					  const rave::Track & ghost_track, bool use_beamspot=false ) const;
    
    /** Reconstruct vertices, using the default reconstruction method.
     *  Two track containers are provided.
     * \paramname primaries Tracks which are used for primary vertex
     * reconstruction only.
     * \paramname tracks Tracks which are subjected to pattern recognition.
     * \paramname use_beamspot defines, if the beamspot information
     * is taken into account.
     */
    std::vector < rave::Vertex > create ( const std::vector < rave::Track > & primaries,
        const std::vector < rave::Track > & tracks, bool use_beamspot=false ) const;

    /** Reconstruct vertices, using the default reconstruction method.
     *  Two track containers are provided.
     * \paramname primaries Tracks which are used for primary vertex
     * reconstruction only.
     * \paramname tracks Tracks which are subjected to pattern recognition.
     * \paramname use_beamspot defines, if the beamspot information
     * is taken into account.
     * \paramname ghost_track defines, if the jetaxis information
     * is taken into account.
     */
    std::vector < rave::Vertex > create ( const std::vector < rave::Track > & primaries,
        const std::vector < rave::Track > & tracks, 
	const rave::Track &ghost_track, bool use_beamspot=false ) const;


    /** Reconstruct vertices, using "method".
     * \paramname use_beamspot defines, if the beamspot information
     * is taken into account.
     */
    std::vector < rave::Vertex > create ( 
        const std::vector < rave::Track > &, 
        const std::string & method, bool use_beamspot=false ) const;
    
    /** Reconstruct vertices, using "method".
     * \paramname use_beamspot defines, if the beamspot information
     * is taken into account.
     * \paramname seed, seed the vertices with this point,
     * look around there.
     */
    std::vector < rave::Vertex > create ( 
        const std::vector < rave::Track > &, 
        const rave::Point3D & seed,
        const std::string & method, bool use_beamspot=false ) const;
    
    
    /** Reconstruct vertices, using "method".
     * \paramname use_beamspot defines, if the beamspot information
     * is taken into account
     * \paramname ghost_track defines, if the jetaxis information
     * is taken into account.
     */
    std::vector < rave::Vertex > create ( const std::vector < rave::Track > &, 
					  const std::string & method, 
					  const rave::Track & ghost_track, bool use_beamspot=false ) const;

    /** Reconstruct vertices, using "method".
     * \paramname use_beamspot defines, if the beamspot information
     * is taken into account.
     * \paramname primaries Tracks which are used for primary vertex
     * reconstruction only.
     * \paramname tracks Tracks which are subjected to pattern recognition.
     */
    std::vector < rave::Vertex > create ( 
        const std::vector < rave::Track > & primaries,
        const std::vector < rave::Track > & tracks,
        const std::string & method, bool use_beamspot=false ) const;

    /** Reconstruct vertices, using "method".
     * \paramname use_beamspot defines, if the beamspot information
     * is taken into account.
     * \paramname primaries Tracks which are used for primary vertex
     * reconstruction only.
     * \paramname tracks Tracks which are subjected to pattern recognition.
     * \paramname ghost_track defines, if the jetaxis information 
     * is taken into account.
     */
    std::vector < rave::Vertex > create ( 
            const std::vector < rave::Track > & primaries,
            const std::vector < rave::Track > & tracks,
            const std::string & method, 
				    const rave::Track & ghost_track, bool use_beamspot=false ) const;

    /// Returns the propagator that is used
    const rave::Propagator & getPropagator() const;

    /// Returns the magnetic field
    const rave::MagneticField & getMagneticField() const;

    /// Returns the beamspot constraint that is used
    const rave::Ellipsoid3D & getBeamspot() const;

    /// True, if the factory knows of a beamspot constraint
    bool hasBeamspot() const;

    int verbosity() const;

    /// Returns the default vertex reconstruction method
    const std::string & method() const;

    /// Set the default vertex reconstruction method
    void setDefaultMethod ( const std::string & );

    /// Set the beam spot constraint.
    void setBeamSpot ( const rave::Ellipsoid3D & );
    
    const rave::VertexReconstructor & reconstructor() const;

    /**
     * a virtual init, for subclasses.
     */
    virtual void init() {};
    void wipe() const; //< wipe InEvent ref counted objects
    
  private:
    void setup();
    std::vector < rave::Vertex > fit ( const std::vector < rave::Track > & prims,
                                 const std::vector < rave::Track > & secs,
                                 const rave::VertexReconstructor &,
                                 const rave::Point3D & seed, bool use_seed, 
                                 bool use_bs, bool use_prims ) const;
    
    std::vector < rave::Vertex > fit ( const std::vector < rave::Track > & prims,
                                 const std::vector < rave::Track > & secs,
                                 const rave::VertexReconstructor &,
                                 const rave::Point3D & seed, bool use_seed,
				 bool use_bs, bool use_prims,
				 const rave::Track & ghost_track ) const;

  private:
    rave::VertexReconstructor * theRector;
    mutable std::map < std::string, rave::VertexReconstructor * > theRectors;

    std::string theMethod;
    rave::MagneticField * theField;
    rave::Propagator * theProp;
    int theVerbosity;
};

}

#endif
