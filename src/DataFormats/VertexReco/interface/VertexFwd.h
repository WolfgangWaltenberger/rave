#ifndef TrackFwdReco_TrackFwd_H
#define TrackFwdReco_TrackFwd_H

#include <vector>
#include "DataFormats/VertexReco/interface/Vertex.h"

namespace reco {
  typedef const reco::Vertex * VertexRef; 
  typedef const reco::Vertex * VertexBaseRef;
  typedef std::vector < VertexRef > VertexRefVector;
}

#endif
