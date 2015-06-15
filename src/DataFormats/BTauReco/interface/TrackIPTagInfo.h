#ifndef BTauReco_TrackIpTagInfo_h
#define BTauReco_TrackIpTagInfo_h

#include "DataFormats/BTauReco/interface/RefMacros.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

namespace reco {

class TrackIPTagInfo
{
  public:

  struct TrackIPData                                                                                                                             {
    Measurement1D ip2d;
    Measurement1D ip3d;
    GlobalPoint closestToJetAxis;
    GlobalPoint closestToFirstTrack;
    float  distanceToJetAxis;
    float  distanceToFirstTrack;
  };

  typedef enum {IP3DSig = 0, Prob3D = 1, IP2DSig = 2, Prob2D =3 ,
                IP3DValue =4, IP2DValue=5 } SortCriteria;
};
}

#endif
