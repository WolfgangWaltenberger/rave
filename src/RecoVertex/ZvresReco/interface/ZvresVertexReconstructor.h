#ifndef _ZvresVertexReconstructor_H_
#define _ZvresVertexReconstructor_H_

#include "RecoVertex/VertexPrimitives/interface/VertexReconstructor.h"

class ZvresVertexReconstructor : public VertexReconstructor {
public:
  ZvresVertexReconstructor();
  ZvresVertexReconstructor( const ZvresVertexReconstructor & o );
  ~ZvresVertexReconstructor();

  std::vector<TransientVertex> vertices(const std::vector<reco::TransientTrack> & v ) const;
  std::vector<TransientVertex> vertices(const std::vector<reco::TransientTrack> & v,
      const reco::BeamSpot & s ) const;

  void setDoubleParameter ( const std::string &, double );

  virtual ZvresVertexReconstructor * clone() const {
    return new ZvresVertexReconstructor( * this );
  }
private:
  void * ZVRES_;
};

#endif
