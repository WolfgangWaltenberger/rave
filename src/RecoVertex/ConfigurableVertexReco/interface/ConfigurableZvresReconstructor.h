#ifndef _ConfigurableZvresReconstructor_H_
#define _ConfigurableZvresReconstructor_H_

#include "RecoVertex/ConfigurableVertexReco/interface/AbstractConfReconstructor.h"
#include "RecoVertex/ZvresReco/interface/ZvresVertexReconstructor.h"

/**
 *  Wrap any VertexFitter into the VertexReconstructor interface
 */

class ConfigurableZvresReconstructor : public AbstractConfReconstructor
{
  public:
    ConfigurableZvresReconstructor ();
    void configure ( const edm::ParameterSet & );
    ConfigurableZvresReconstructor ( const ConfigurableZvresReconstructor & o );
    ~ConfigurableZvresReconstructor();
    ConfigurableZvresReconstructor * clone () const;
    std::vector < TransientVertex > vertices ( 
        const std::vector < reco::TransientTrack > & t ) const;
    std::vector < TransientVertex > vertices ( 
        const std::vector < reco::TransientTrack > & t,
        const reco::BeamSpot & s ) const;
    edm::ParameterSet defaults() const;
  private:
    const ZvresVertexReconstructor * theRector;
};

#endif
