#ifndef _ConfigurableMultiVertexBSeeder_H_
#define _ConfigurableMultiVertexBSeeder_H_

#include "RecoVertex/ConfigurableVertexReco/interface/AbstractConfReconstructor.h"

class MultiVertexBSeeder;

/**
 *  Wrap any VertexFitter into the VertexReconstructor interface
 */

class ConfigurableMultiVertexBSeeder : public AbstractConfReconstructor
{
  public:
    /**
     * Accepted values:
     *  decaylength: 1.5 will be obsolete soon
     */
    ConfigurableMultiVertexBSeeder ();
    ConfigurableMultiVertexBSeeder ( const ConfigurableMultiVertexBSeeder & o );
    ~ConfigurableMultiVertexBSeeder();
    ConfigurableMultiVertexBSeeder * clone () const;
    std::vector < TransientVertex > vertices ( 
        const std::vector < reco::TransientTrack > & t ) const;
    std::vector < TransientVertex > vertices ( 
        const std::vector < reco::TransientTrack > & t,
        const reco::BeamSpot & s ) const;
    std::vector < TransientVertex > vertices ( 
        const std::vector < reco::TransientTrack > & t,
        const reco::BeamSpot & s, 
	const reco::TransientTrack & g) const;
    void configure ( const edm::ParameterSet & );
    edm::ParameterSet defaults() const;
  private:
    const MultiVertexBSeeder * theRector;
};

#endif
