#include "RecoVertex/LinearizationPointFinders/interface/GenericLinearizationPointFinder.h"
#include "RecoVertex/LinearizationPointFinders/interface/FallbackLinearizationPointFinder.h"

GenericLinearizationPointFinder::~GenericLinearizationPointFinder()
{
  delete theFitter;
}

GlobalPoint GenericLinearizationPointFinder::getLinearizationPoint(
    const std::vector< reco::TransientTrack > & tracks ) const
{
  try {
    return theFitter->vertex ( tracks ).position();
  } catch ( ... ) {};
  FallbackLinearizationPointFinder linpt;
  return linpt.getLinearizationPoint ( tracks );
  // return GlobalPoint(0.,0.,0.);
}

LinearizationPointFinder * GenericLinearizationPointFinder::clone() const
{
  return new GenericLinearizationPointFinder(* this);
}

