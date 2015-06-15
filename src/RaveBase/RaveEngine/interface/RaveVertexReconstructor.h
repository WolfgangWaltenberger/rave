#ifndef _RaveVertexReconstructor_H_
#define _RaveVertexReconstructor_H_

#include <string>
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/VertexReconstructor.h"
#include <vector>

/**
 *  The Rave Reconstructor, configured by a string.
 *  What happens is this.
 *  The RaveVertexReconstructor calls RaveStrategyBuilder.
 *  The builder parses the string, splits it up into map < string, MultiType >.
 *  Then it calls ConfigurableVertexReconstructor with this map.
 *  And since this is so little, I think the RaveVertexReconstructor might
 *  better get absorbed in the rave::Factory.
 */

namespace rave {
class VertexReconstructor : public ::VertexReconstructor
{
  public:
    VertexReconstructor ( const std::string & );
    VertexReconstructor ( const VertexReconstructor & );
    ~VertexReconstructor();
    std::vector < TransientVertex > vertices ( const std::vector < reco::TransientTrack > &, const GlobalPoint & seed ) const;
    std::vector < TransientVertex > vertices ( const std::vector < reco::TransientTrack > & ) const;
    std::vector < TransientVertex > vertices ( const std::vector < reco::TransientTrack > &,
        const reco::TransientTrack & ) const;
    std::vector < TransientVertex > vertices ( const std::vector < reco::TransientTrack > &,
	const reco::BeamSpot & s, const reco::TransientTrack & ) const;
    std::vector < TransientVertex > vertices ( const std::vector < reco::TransientTrack > &,
        const reco::BeamSpot & s ) const;
    std::vector < TransientVertex > vertices ( const std::vector < reco::TransientTrack > & prims,
        const std::vector < reco::TransientTrack > & secs,
        const reco::BeamSpot & s ) const;
    std::vector < TransientVertex > vertices ( const std::vector < reco::TransientTrack > & prims,
        const std::vector < reco::TransientTrack > & secs,
	const reco::BeamSpot & s,
	const reco::TransientTrack & ghost_track) const;

    VertexReconstructor * clone() const;

    const std::string & method() const;

  private:
    const ::VertexReconstructor * theRector;
    std::string theMethod;
};
}

#endif
