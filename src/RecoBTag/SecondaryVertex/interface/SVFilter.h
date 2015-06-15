#include "RecoBTag/SecondaryVertex/interface/SecondaryVertex.h"
#include "RecoBTag/SecondaryVertex/interface/VertexFilter.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

class SVFilter : public std::unary_function<const reco::SecondaryVertex&, bool>
{
  public:
    SVFilter( const reco::VertexFilter & filter, const reco::Vertex & pv,
              const GlobalVector & direction );

    inline bool operator () ( const reco::SecondaryVertex & sv ) const
          { return !filter(pv, sv, direction); };

  private:
    const reco::VertexFilter & filter;
    const reco::Vertex & pv;
    const GlobalVector & direction;
};

