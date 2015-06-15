#ifndef _CmsToRaveObjects_H_
#define _CmsToRaveObjects_H_

#include <rave/Track.h>
#include <rave/BasicTrack.h>
#include <rave/Vertex.h>
#include <rave/Vector3D.h>
#include <rave/Vector6D.h>
#include <rave/Covariance3D.h>
#include <rave/Covariance33D.h>
#include <rave/Covariance6D.h>
#include <rave/PerigeeParameters5D.h>
#include <rave/PerigeeCovariance5D.h>
#include <rave/Plane.h>
#include <rave/Cylinder.h>
#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "RecoVertex/VertexPrimitives/interface/CachingVertex.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"

#ifdef WITH_KINEMATICS

#include <rave/Vector7D.h>
#include <rave/Covariance7D.h>

#include <rave/PerigeeParameters6D.h>
#include <rave/PerigeeCovariance6D.h>

#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParameters.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParametersError.h"

#include "RecoVertex/KinematicFitPrimitives/interface/ExtendedPerigeeTrajectoryParameters.h"
#include "RecoVertex/KinematicFitPrimitives/interface/ExtendedPerigeeTrajectoryError.h"

#endif

class GlobalTrajectoryParameters;
class CartesianTrajectoryError;

class CmsToRaveObjects 
{
  public:
    rave::Track convert ( const TrajectoryStateOnSurface &, float chi2=0., float ndf=0., 
                          void * orig=0, const std::string & tag="" ) const;
    rave::Track convert ( int id, const TrajectoryStateOnSurface &, float chi2=0., float ndf=0., 
                          void * orig=0, const std::string & tag="" ) const;
    rave::BasicTrack toBasicTrack ( const TrajectoryStateOnSurface &, float chi2=0., float ndf=0., 
                          void * orig=0, const std::string & tag="" ) const;
    rave::Track convert ( const FreeTrajectoryState & ) const;
    rave::Track convert ( const FreeTrajectoryState &, float chi2, float ndf, void * id, const std::string & tag ) const;
    rave::Track convert ( int id, const FreeTrajectoryState &, float chi2, float ndf, void * orig, const std::string & tag ) const;
    rave::Track convert ( const reco::TransientTrack & t, int id=0 ) const;
    rave::Vector3D convert ( const GlobalPoint & ) const;
    rave::Vector3D convert ( const GlobalVector & ) const;
    rave::Vector6D convert ( const GlobalTrajectoryParameters & ) const;
    rave::Covariance3D convert ( const GlobalError & ) const;
    rave::Covariance6D convert ( const CartesianTrajectoryError & ) const;
    ravesurf::Plane convert ( const Plane & ) const;
    ravesurf::Cylinder convert ( const Cylinder & ) const;
    rave::Vertex convert( const TransientVertex& tv, const std::vector < rave::Track > & ) const;
    /*rave::Vertex convertCV ( const CachingVertex<5> & cv, 
                             const std::vector < rave::Track > & ) const;*/

    rave::PerigeeParameters5D convert(const PerigeeTrajectoryParameters &) const;
    rave::PerigeeCovariance5D convert(const PerigeeTrajectoryError &) const;
    rave::Covariance33D convert ( const AlgebraicMatrix3M & ) const;

    #ifdef WITH_KINEMATICS
    rave::Vector7D convert(const ::KinematicParameters &) const;
    rave::Covariance7D convert(const ::KinematicParametersError &) const;

    rave::PerigeeParameters6D convert(const ExtendedPerigeeTrajectoryParameters &) const;
    rave::PerigeeCovariance6D convert(const ExtendedPerigeeTrajectoryError &) const;
    #endif
};

#endif
