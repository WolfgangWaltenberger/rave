#ifndef GenericLinearizationPointFinder_H
#define GenericLinearizationPointFinder_H

#include "RecoVertex/VertexTools/interface/LinearizationPointFinder.h"
#include "RecoVertex/VertexPrimitives/interface/VertexFitter.h"

  /** A generic linearization point finder, that uses the result of 
   *  a Fitter to be used as a lin.point
   */

class GenericLinearizationPointFinder : public LinearizationPointFinder {

public:

  GenericLinearizationPointFinder ( const VertexFitter<5> & fitter ) : 
    theFitter ( fitter.clone() ) {}

  ~GenericLinearizationPointFinder ();

  /** Method giving back the Initial Linearization Point.
   */
  virtual 
  GlobalPoint getLinearizationPoint(const vector<reco::TransientTrack> & tracks) const;

  /** Clone method
   */        
  virtual LinearizationPointFinder * clone() const;

private:

  const VertexFitter<5> * theFitter;

};

#endif
