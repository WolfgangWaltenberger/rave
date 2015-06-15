#include "RecoBTag/SecondaryVertex/interface/SVBuilder.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"

using namespace reco;

SVBuilder::SVBuilder(const Vertex &pv,
              const GlobalVector &direction,
              bool withPVError) :
      pv(pv), direction(direction),
      withPVError(withPVError) {}
    
SecondaryVertex SVBuilder::operator () (const Vertex &sv) const
    { return SecondaryVertex(pv, sv, direction, withPVError); }
