#ifndef TTtoVtxmap_H
#define TTtoVtxmap_H

#include <map>
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

/** Map of (key = TransientTrack
 *          elt = map of(key = TransientTrack, elt = covariance matrix))
 */

typedef ROOT::Math::SMatrix<double,3,3,ROOT::Math::MatRepStd<double,3,3> > 
        AlgebraicMatrix3M;
typedef std::map<reco::TransientTrack, AlgebraicMatrix3M> TTtoVtxmap;


#endif
