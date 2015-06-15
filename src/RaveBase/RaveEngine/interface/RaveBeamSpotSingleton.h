#ifndef _RaveBeamSpotSingleton_H_
#define _RaveBeamSpotSingleton_H_

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include <rave/Ellipsoid3D.h>


/**
 *  A singleton to provide the BeamSpot globally to
 *  all RAVE facilities.
 */

namespace rave {
class BeamSpotSingleton
{
  public:
    static const reco::BeamSpot * instance();
    static bool hasBeamSpot();
    static const rave::Ellipsoid3D & get();
    
    static void set ( const rave::Ellipsoid3D & );
};
}

#endif
