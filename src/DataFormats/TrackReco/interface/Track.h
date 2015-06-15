#ifndef TrackReco_Track_h
#define TrackReco_Track_h

#include "DataFormats/TrackReco/interface/TrackBase.h"

namespace reco {
  class Track : public TrackBase {
  public:
        Track() { } 
        Track( double chi2, double ndof, const Point & referencePoint,
               const Vector & momentum, int charge, const CovarianceMatrix &,
               TrackAlgorithm=undefAlgorithm, TrackQuality quality=undefQuality);

        virtual ~Track();

  };

}

#endif
