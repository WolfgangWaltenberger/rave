#ifndef _ConfigurableSkeletonReconstructor_H_
#define _ConfigurableSkeletonReconstructor_H_

#include "RecoVertex/ConfigurableVertexReco/interface/AbstractConfReconstructor.h"

/**
 *   a minimal ConfigurableSkeletonReconstructor,
 *   meant to get you started with developing your own VertexReconstructor.
 *   See also ./src/RecoVertex/SkeletonVertexReco and 
 *   ./src/RecoVertex/Makefile.am
 *
 */

class ConfigurableSkeletonReconstructor : public AbstractConfReconstructor
{
  public:
    ConfigurableSkeletonReconstructor ();
    void configure ( const edm::ParameterSet & );
    ConfigurableSkeletonReconstructor ( const ConfigurableSkeletonReconstructor & o );
    ~ConfigurableSkeletonReconstructor();
    ConfigurableSkeletonReconstructor * clone () const;
    std::vector < TransientVertex > vertices ( 
        const std::vector < reco::TransientTrack > & t ) const;
    edm::ParameterSet defaults() const;
  private:
    const VertexReconstructor * theRector;
};

#endif
