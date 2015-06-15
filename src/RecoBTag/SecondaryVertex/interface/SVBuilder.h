#include "RecoBTag/SecondaryVertex/interface/SecondaryVertex.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

class SVBuilder  : public std::unary_function<const reco::Vertex&, reco::SecondaryVertex>
{
  public:
    SVBuilder(const reco::Vertex &pv,
              const GlobalVector &direction,
              bool withPVError);

    reco::SecondaryVertex operator () (const reco::Vertex & sv) const;

  private:
    const reco::Vertex & pv;
    const GlobalVector & direction;
    bool withPVError;
};
