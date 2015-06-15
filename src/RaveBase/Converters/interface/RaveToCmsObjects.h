#ifndef _RaveToCmsObjects_H_
#define _RaveToCmsObjects_H_

#include <rave/Track.h>
#include <rave/Vertex.h>
#include <rave/Covariance6D.h>
#include <rave/Plane.h>
#include <rave/Cylinder.h>

#include "TrackingTools/TrajectoryParametrization/interface/CartesianTrajectoryError.h"
#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/GlobalError.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"

#ifdef WITH_KINEMATICS

#include <rave/Vector4D.h>
#include <rave/Vector7D.h>
#include <rave/Covariance4D.h>
#include <rave/Covariance7D.h>

#include <rave/PerigeeParameters6D.h>
#include <rave/PerigeeCovariance6D.h>

#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParameters.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParametersError.h"

#include "RecoVertex/KinematicFitPrimitives/interface/ExtendedPerigeeTrajectoryParameters.h"
#include "RecoVertex/KinematicFitPrimitives/interface/ExtendedPerigeeTrajectoryError.h"

#endif

class RaveToCmsObjects
{
  public:
    GlobalTrajectoryParameters convert ( const rave::Track & ) const;
    GlobalTrajectoryParameters convert ( const rave::Vector6D &, const rave::Charge & ) const;
    TransientVertex convert ( const rave::Vertex & ) const;
    CartesianTrajectoryError convert ( const rave::Covariance6D & ) const;
    GlobalPoint convert ( const rave::Vector3D & ) const;
    GlobalVector toVector ( const rave::Vector3D & ) const;
    AlgebraicVector toAlgebraicVector ( const rave::Vector3D & ) const;
    AlgebraicVector3 toAlgebraicVector3 ( const rave::Vector3D & ) const;
    GlobalError convert ( const rave::Covariance3D & ) const;
    reco::TransientTrack tTrack ( const rave::Track & ) const;
    FreeTrajectoryState convertTrackToFTS ( const rave::Track & ) const;

    PerigeeTrajectoryParameters convert(const rave::PerigeeParameters5D &, const rave::Charge &) const;
    PerigeeTrajectoryError convert(const rave::PerigeeCovariance5D &) const;
    Plane convert ( const ravesurf::Plane & ) const;
    Cylinder convert ( const ravesurf::Cylinder & ) const;

    #ifdef WITH_KINEMATICS
    AlgebraicVector toAlgebraicVector(const rave::Vector4D &) const;
    ::KinematicParameters convert(const rave::Vector7D &) const;
    AlgebraicSymMatrix toAlgebraicSymMatrix(const rave::Covariance4D &) const;
    ::KinematicParametersError convert(const rave::Covariance7D &) const;

    ExtendedPerigeeTrajectoryParameters convert(
        const rave::PerigeeParameters6D &, const rave::Charge &) const;
    ExtendedPerigeeTrajectoryError convert(
        const rave::PerigeeCovariance6D &) const;
    #endif
};

#endif
