#ifndef _ConfigurableTertiaryTracksFinder_H_
#define _ConfigurableTertiaryTracksFinder_H_

#include "RecoVertex/ConfigurableVertexReco/interface/AbstractConfReconstructor.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"

class ConfigurableTertiaryTracksVertexFinder;

/**
 *  Wrap any VertexFitter into the VertexReconstructor interface
 */

class ConfigurableTertiaryTracksFinder : public AbstractConfReconstructor
{
  public:
    /**
     *
     * Accepted values:
     *  sigmacut: The sqrt(chi2_cut) criterion. Default: 3.0
     *  ratio:   The annealing ratio. Default: 0.25
     *  Tini:    The initial temparature. Default: 256
     *
     */

    ConfigurableTertiaryTracksFinder ();
    ConfigurableTertiaryTracksFinder ( const ConfigurableTertiaryTracksFinder & o );
    ~ConfigurableTertiaryTracksFinder();
    ConfigurableTertiaryTracksFinder * clone () const;
    std::vector < TransientVertex > vertices ( 
        const std::vector < reco::TransientTrack > & t ) const;
    std::vector < TransientVertex > vertices ( 
        const std::vector < reco::TransientTrack > & t,
        const reco::BeamSpot & s ) const;
    void configure ( const edm::ParameterSet & );
    edm::ParameterSet defaults() const;
  private:
    const ConfigurableTertiaryTracksVertexFinder * theRector;
    const AdaptiveVertexFitter * thePrimaryFitter;
};

#endif
