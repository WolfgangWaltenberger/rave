#include "RecoBTag/SecondaryVertex/interface/SVFilter.h"

using namespace reco;

SVFilter::SVFilter(const VertexFilter &filter, const Vertex &pv,
             const GlobalVector & direction) :
    filter(filter), pv(pv), direction(direction) {}
