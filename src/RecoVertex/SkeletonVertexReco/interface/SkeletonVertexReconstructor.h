#ifndef _SkeletonVertexReconstructor_H_
#define _SkeletonVertexReconstructor_H_

#include "RecoVertex/VertexPrimitives/interface/VertexReconstructor.h"

class SkeletonVertexReconstructor : public VertexReconstructor {
public:

  /***
   * A minimal skeleton class, just to get you started with
   * your own vertex reconstructor.
   * See also ./src/RecoVertex/ConfigurableVertexReco/src/
   * ConfigurableSkeletonReconstructor.cc,
   * and ./src/RecoVertex/Makefile.am.
   */

  std::vector<TransientVertex> vertices(
      const std::vector<reco::TransientTrack> & v ) const;
  
  virtual SkeletonVertexReconstructor * clone() const {
    return new SkeletonVertexReconstructor( * this );
  }
};

#endif
