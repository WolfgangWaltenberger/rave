#ifndef _RaveInterface_FlavorTagFactory_H_
#define _RaveInterface_FlavorTagFactory_H_

#include <rave/Track.h>
#include <rave/Vertex.h>
#include <rave/Ellipsoid3D.h>
#include <rave/MagneticField.h>
#include <rave/VertexFactory.h>
#include <string>

namespace rave
{
  class VertexReconstructor;

/** 
 *  The class that is central to using the rave library.
 *  It builds the RaveVertices from a set of RaveTracks.
 *  \paramname method is a string that describes which
 *  algorithms should be employed and which parameter
 *  settings should be chosen.
 *  The default is chosen such that something
 *  "sensible" is done in most use cases.
 */

class RaveDllExport FlavorTagFactory 
{
  public:
    /**
     *  Create a flavortagfactory supplying the name of the method employed
     *  (currently unused) and the vertexingmethod, just like
     *  the VertexFactory would have it
     *  (e.g. "avr-primcut:3.0"). Need to provide a magnetic field, also.
     */
    FlavorTagFactory( const rave::MagneticField & f,
                      const std::string & method="default",
                      const std::string & vertexingmethod="default" );
    /**
     *  Convenience constructor, for the case that a vertex factory
     *  exists, already.
     */
    FlavorTagFactory( const rave::VertexFactory & f,
                      const std::string & method="default" );

    ~FlavorTagFactory();

    /**
     *  Tag, giving primary vertex, tracks in the jet cone,
     *  plus jet cone direction.
     *  This interface most closely resembles the cmssw interface.
     */
    float tag ( const std::vector < rave::Track > & tracks,
                const rave::Vertex & primary, 
                const rave::Vector3D & jet ) const;

    bool hasFlavorTagging() const; //< has btagging code?

    void resetBeamSpot(); //< call this if beamspot changes.

  private:
    void setup ( const std::string & method, const std::string & vertexingmethod );
    FlavorTagFactory ( const FlavorTagFactory & o );
    void * theAlgo; //< keep the algorithm as a data member.
};

}

#endif
