#ifndef TrackReco_TrackFwd_H
#define TrackReco_TrackFwd_H

#include <vector>
// #include "DataFormats/TrackReco/interface/Track.h"
// #include "TrackingTools/TransientTrack/interface/TransientTrack.h"

// A heavily modified rave class

namespace reco {
  class Track;
  class TrackBase;
  typedef const reco::Track * TrackRef; // FIXME TrackRef.isNonnull() would be nice
  typedef const reco::TrackBase * TrackBaseRef; // FIXME TrackRef.isNonnull() would be nice
  typedef std::vector < TrackRef > TrackRefVector;
  typedef TrackRefVector::const_iterator track_iterator;
}

#endif
