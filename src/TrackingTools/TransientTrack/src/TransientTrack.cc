#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TrackTransientTrack.h"

#include <iostream>

using namespace reco;

typedef TrackTransientTrack                              TTT;

TransientTrack::TransientTrack() : 
  Base( new TTT()) {}

TransientTrack::TransientTrack( const Track & tk , const MagneticField* field) : 
  Base( new TTT(tk, field)) {}


TransientTrack::TransientTrack( const TrackRef & tk , const MagneticField* field) : 
  Base( new TTT(tk, field)) {}

