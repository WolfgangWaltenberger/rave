# JAVAC_FLAGS is needed, to enable -fno-strict-aliasing when interfacing to java.
AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/src/ROOT/mathcore/ -I$(top_srcdir)/src/ROOT/genvector/ -I$(top_srcdir)/src/ROOT/smatrix/ -I$(top_srcdir)/src/ROOT/ -I$(top_srcdir)/src/RaveBase/RaveInterface -I$(CLHEPINCPATH) $(BOOST_CPPFLAGS) -DRAVE $(FLAVORTAGGING_CPPFLAGS) $(JAVAC_FLAGS) $(PLATFORMCPPFLAGS)

RecoVertex/libRaveVertex.la:
	cd RecoVertex && $(MAKE) libRaveVertex.la

EXTRA_DIST = boost

# tests
TESTS = UtilitiesTest StatTest

check_PROGRAMS = UtilitiesTest StatTest

UtilitiesTest_SOURCES = FWCore/Utilities/test/Utilities.cpp
UtilitiesTest_LDADD = $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) libRaveCore.la RecoVertex/libRaveVertex.la

#broken
#ConfigurationTest_SOURCES = Environment/Configuration/test/ConfigurationTest.cpp
#ConfigurationTest_LDADD = lib.la

StatTest_SOURCES = CommonTools/Statistics/test/StatTest.cpp
StatTest_LDADD = libRaveCore.la RecoVertex/libRaveVertex.la

# library
lib_LTLIBRARIES = libRaveCore.la

libRaveCore_la_LDFLAGS = $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) $(PLATFORMLDFLAGS)
## libRaveCore_la_LIBADD = $(CLHEPLIB)

libRaveCore_la_SOURCES = \
ROOT/mathcore/TMath.cc \
ROOT/src/GenVector_Throw.cc \
CommonTools/Statistics/interface/hsm_1d.icc \
CommonTools/Statistics/interface/lms_1d.icc \
CommonTools/Statistics/src/GammaSeries.h \
CommonTools/Statistics/src/GammaLn.h \
CommonTools/Statistics/src/GammaContinuedFraction.h \
CommonTools/Statistics/src/IncompleteGammaComplement.h \
TrackingTools/GeomPropagators/src/RealQuadEquation.h \
CommonTools/Clustering1D/src/Cluster1D.cc \
CommonTools/Clustering1D/src/Clustering1DException.cc \
CommonTools/Clustering1D/src/Clusterizer1D.cc \
CommonTools/Clustering1D/src/Clusterizer1DCommons.cc \
CommonTools/Clustering1D/src/FsmwClusterizer1D.cc \
CommonTools/Clustering1D/src/OutermostClusterizer1D.cc \
CommonTools/Clustering1D/src/TrackDistanceWeightEstimator.cc \
CommonTools/Clustering1D/src/TrivialWeightEstimator.cc \
CommonTools/Clustering1D/src/WeightEstimator.cc \
CommonTools/Statistics/src/ChiSquaredProbability.cc \
CommonTools/Statistics/src/GammaContinuedFraction.cc \
CommonTools/Statistics/src/GammaLn.cc \
CommonTools/Statistics/src/GammaSeries.cc \
CommonTools/Statistics/src/IncompleteGammaComplement.cc \
CommonTools/Statistics/src/StatisticsException.cc \
DataFormats/BeamSpot/src/BeamSpot.cc \
DataFormats/Candidate/src/Particle.cc \
DataFormats/TrackReco/src/TrackBase.cc \
DataFormats/TrackReco/src/Track.cc \
DataFormats/TrackReco/src/fillCovariance.cc \
DataFormats/GeometryCommonDetAlgo/src/PerpendicularBoundPlaneBuilder.cc \
DataFormats/GeometrySurface/src/BlockWipedAllocator.cc \
DataFormats/GeometrySurface/src/BoundCone.cc \
DataFormats/GeometrySurface/src/BoundCylinder.cc \
DataFormats/GeometrySurface/src/BoundDisk.cc \
DataFormats/GeometrySurface/src/BoundingBox.cc \
DataFormats/GeometrySurface/src/BoundPlane.cc \
DataFormats/GeometrySurface/src/Cone.cc \
DataFormats/GeometrySurface/src/Cylinder.cc \
DataFormats/GeometrySurface/src/GeneralNSurfaceDelimitedBounds.cc \
DataFormats/GeometrySurface/src/GeneralTrapezoidalPlaneBounds.cc \
DataFormats/GeometrySurface/src/LocalError.cc \
DataFormats/GeometrySurface/src/Plane.cc \
DataFormats/GeometrySurface/src/SimpleDiskBounds.cc \
DataFormats/GeometrySurface/src/TrapezoidalPlaneBounds.cc \
DataFormats/VertexReco/src/Vertex.cc \
Environment/Configuration/src/MultiType.cc \
MagneticField/Engine/src/MagneticField.cc \
TrackingTools/AnalyticalJacobians/src/AnalyticalCurvilinearJacobian.cc \
TrackingTools/AnalyticalJacobians/src/JacobianCartesianToCurvilinear.cc \
TrackingTools/AnalyticalJacobians/src/JacobianCartesianToLocal.cc \
TrackingTools/AnalyticalJacobians/src/JacobianCurvilinearToCartesian.cc \
TrackingTools/AnalyticalJacobians/src/JacobianCurvilinearToLocal.cc \
TrackingTools/AnalyticalJacobians/src/JacobianLocalToCartesian.cc \
TrackingTools/AnalyticalJacobians/src/JacobianLocalToCurvilinear.cc \
TrackingTools/GeomPropagators/src/AnalyticalPropagator.cc \
TrackingTools/GeomPropagators/src/AnalyticalTrajectoryExtrapolatorToLine.cc \
TrackingTools/GeomPropagators/src/HelixArbitraryPlaneCrossing2Order.cc \
TrackingTools/GeomPropagators/src/HelixArbitraryPlaneCrossing.cc \
TrackingTools/GeomPropagators/src/HelixBarrelCylinderCrossing.cc \
TrackingTools/GeomPropagators/src/HelixBarrelPlaneCrossing2OrderLocal.cc \
TrackingTools/GeomPropagators/src/HelixBarrelPlaneCrossingByCircle.cc \
TrackingTools/GeomPropagators/src/HelixExtrapolatorToLine2Order.cc \
TrackingTools/GeomPropagators/src/HelixForwardPlaneCrossing.cc \
TrackingTools/GeomPropagators/src/IterativeHelixExtrapolatorToLine.cc \
TrackingTools/GeomPropagators/src/PropagationDirectionChooser.cc \
TrackingTools/GeomPropagators/src/Propagator.cc \
TrackingTools/GeomPropagators/src/StraightLineBarrelCylinderCrossing.cc \
TrackingTools/GeomPropagators/src/StraightLineCylinderCrossing.cc \
TrackingTools/GeomPropagators/src/StraightLinePlaneCrossing.cc \
TrackingTools/PatternTools/src/TransverseImpactPointExtrapolator.cc \
TrackingTools/PatternTools/src/TSCPBuilderNoMaterial.cc \
TrackingTools/PatternTools/src/TrajectoryStateClosestToBeamLineBuilder.cc \
TrackingTools/PatternTools/src/TSCBLBuilderNoMaterial.cc \
TrackingTools/PatternTools/src/ClosestApproachInRPhi.cc \
TrackingTools/PatternTools/src/TwoTrackMinimumDistance.cc \
TrackingTools/PatternTools/src/TwoTrackMinimumDistanceHelixHelix.cc \
TrackingTools/PatternTools/src/TwoTrackMinimumDistanceHelixLine.cc \
TrackingTools/PatternTools/src/TwoTrackMinimumDistanceLineLine.cc \
TrackingTools/TrajectoryParametrization/src/CartesianTrajectoryError.cc \
TrackingTools/TrajectoryParametrization/src/GlobalTrajectoryParameters.cc \
TrackingTools/TrajectoryParametrization/src/LocalTrajectoryError.cc \
TrackingTools/TrajectoryParametrization/src/PerigeeTrajectoryError.cc \
TrackingTools/TrajectoryState/src/BasicSingleTrajectoryState.cc \
TrackingTools/TrajectoryState/src/BasicTrajectoryState.cc \
TrackingTools/TrajectoryState/src/FreeTrajectoryState.cc \
TrackingTools/TrajectoryState/src/PerigeeConversions.cc \
TrackingTools/TrajectoryState/src/TrajectoryStateClosestToPoint.cc \
TrackingTools/TrajectoryState/src/TrajectoryStateClosestToBeamLine.cc \
TrackingTools/TrajectoryState/src/TrajectoryStateOnSurface.cc \
TrackingTools/TrajectoryState/src/FreeTrajectoryStatePrint.cc \
TrackingTools/TrajectoryState/src/TrajectoryStateOnSurfacePrint.cc \
TrackingTools/TrajectoryState/src/TrajectoryStateTransform.cc \
TrackingTools/TransientTrack/src/TransientTrackBuilder.cc \
TrackingTools/TransientTrack/src/TransientTrackFromFTSFactory.cc \
TrackingTools/TransientTrack/src/TransientTrackFromFTS.cc \
TrackingTools/TransientTrack/src/TransientTrack.cc \
TrackingTools/TransientTrack/src/TrackTransientTrack.cc \
RaveTools/Converters/src/MagneticFieldSingleton.cc \
RaveTools/Converters/src/PropagatorSingleton.cc \
FWCore/MessageLogger/src/Colors.cc \
FWCore/MessageLogger/src/MessageLogger.cc \
FWCore/ParameterSet/src/ParameterSet.cc \
FWCore/Utilities/src/EDMException.cc \
FWCore/Utilities/src/Exception.cc

nobase_implinclude_HEADERS = \
CommonTools/Statistics/interface/ChiSquaredProbability.h \
DataFormats/BeamSpot/interface/BeamSpot.h \
DataFormats/Candidate/interface/Particle.h \
DataFormats/CLHEP/interface/AlgebraicObjects.h \
DataFormats/CLHEP/interface/Migration.h \
ROOT/Rtypes.h \
ROOT/smatrix/Math/SMatrix.h \
ROOT/smatrix/Math/SVector.h \
ROOT/smatrix/Math/SMatrix.icc \
ROOT/smatrix/Math/Dfact.h \
ROOT/smatrix/Math/Dinv.h \
ROOT/smatrix/Math/CramerInversion.icc \
ROOT/smatrix/Math/CramerInversionSym.icc \
ROOT/smatrix/Math/MatrixInversion.icc \
ROOT/smatrix/Math/HelperOps.h \
ROOT/smatrix/Math/BinaryOpPolicy.h \
ROOT/smatrix/Math/SVector.icc \
ROOT/smatrix/Math/UnaryOperators.h \
ROOT/smatrix/Math/BinaryOperators.h \
ROOT/smatrix/Math/Functions.h \
ROOT/smatrix/Math/MConfig.h \
ROOT/smatrix/Math/Expression.h \
ROOT/smatrix/Math/MatrixRepresentationsStatic.h \
ROOT/smatrix/Math/MatrixFunctions.h \
ROOT/smatrix/Math/StaticCheck.h \
ROOT/genvector/Math/Point3D.h \
ROOT/genvector/Math/Point3Dfwd.h \
ROOT/genvector/Math/Vector3D.h \
ROOT/genvector/Math/Vector3Dfwd.h \
ROOT/mathcore/Math/Math.h \
ROOT/genvector/Math/GenVector/GenVector_exception.h \
ROOT/genvector/Math/GenVector/Cartesian3D.h \
ROOT/genvector/Math/GenVector/CylindricalEta3D.h \
ROOT/genvector/Math/GenVector/Polar3D.h \
ROOT/genvector/Math/GenVector/eta.h \
ROOT/genvector/Math/GenVector/etaMax.h \
ROOT/genvector/Math/GenVector/Polar3Dfwd.h \
ROOT/genvector/Math/GenVector/Cylindrical3D.h \
ROOT/genvector/Math/GenVector/PositionVector3D.h \
ROOT/genvector/Math/GenVector/PositionVector3Dfwd.h \
ROOT/genvector/Math/GenVector/DisplacementVector3Dfwd.h \
ROOT/genvector/Math/GenVector/GenVectorIO.h\
ROOT/genvector/Math/GenVector/BitReproducible.h \
ROOT/genvector/Math/GenVector/CoordinateSystemTags.h \
ROOT/genvector/Math/GenVector/DisplacementVector3D.h \
DataFormats/GeometryCommonDetAlgo/interface/DeepCopyPointer.h \
DataFormats/GeometryCommonDetAlgo/interface/DeepCopyPointerByClone.h \
DataFormats/GeometryCommonDetAlgo/interface/ErrorMatrixTag.h \
DataFormats/GeometryCommonDetAlgo/interface/GlobalErrorBase.h \
DataFormats/GeometryCommonDetAlgo/interface/GlobalError.h \
DataFormats/GeometryCommonDetAlgo/interface/GlobalWeight.h \
DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h \
DataFormats/GeometryCommonDetAlgo/interface/PerpendicularBoundPlaneBuilder.h \
DataFormats/GeometryCommonDetAlgo/interface/WeightMatrixTag.h \
DataFormats/GeometrySurface/interface/Bounds.h \
DataFormats/GeometrySurface/interface/BoundPlane.h \
DataFormats/GeometrySurface/interface/BoundSurface.h \
DataFormats/GeometrySurface/interface/Cylinder.h \
DataFormats/GeometrySurface/interface/GloballyPositioned.h \
DataFormats/GeometrySurface/interface/LocalError.h \
DataFormats/GeometrySurface/interface/Plane.h \
DataFormats/GeometrySurface/interface/ReferenceCounted.h \
DataFormats/GeometrySurface/interface/Surface.h \
DataFormats/GeometrySurface/interface/MediumProperties.h \
DataFormats/GeometrySurface/interface/TkRotation.h \
DataFormats/GeometryVector/interface/Basic2DVector.h \
DataFormats/GeometryVector/interface/Basic3DVector.h \
DataFormats/GeometryVector/interface/CoordinateSets.h \
DataFormats/GeometryVector/interface/GlobalPoint.h \
DataFormats/GeometryVector/interface/GlobalTag.h \
DataFormats/GeometryVector/interface/GlobalVector.h \
DataFormats/GeometryVector/interface/LocalPoint.h \
DataFormats/GeometryVector/interface/LocalTag.h \
DataFormats/GeometryVector/interface/LocalVector.h \
DataFormats/GeometryVector/interface/Phi.h \
DataFormats/GeometryVector/interface/Pi.h \
DataFormats/GeometryVector/interface/Point2DBase.h \
DataFormats/GeometryVector/interface/Point3DBase.h \
DataFormats/GeometryVector/interface/PointTag.h \
DataFormats/GeometryVector/interface/PreciseFloatType.h \
DataFormats/GeometryVector/interface/PV2DBase.h \
DataFormats/GeometryVector/interface/PV3DBase.h \
DataFormats/GeometryVector/interface/Theta.h \
DataFormats/GeometryVector/interface/Vector2DBase.h \
DataFormats/GeometryVector/interface/Vector3DBase.h \
DataFormats/GeometryVector/interface/VectorTag.h \
DataFormats/Common/interface/RefToBase.h \
DataFormats/Math/interface/Error.h \
DataFormats/Math/interface/Point3D.h \
DataFormats/Math/interface/Vector3D.h \
DataFormats/Math/interface/Vector.h \
DataFormats/Math/interface/LorentzVector.h \
DataFormats/TrackReco/interface/TrackBase.h \
DataFormats/TrackReco/interface/TrackFwd.h \
DataFormats/TrackReco/interface/Track.h \
DataFormats/TrajectorySeed/interface/PropagationDirection.h \
DataFormats/TrajectoryState/interface/LocalTrajectoryParameters.h \
DataFormats/TrajectoryState/interface/TrackCharge.h \
DataFormats/VertexReco/interface/Vertex.h \
DetectorDescription/Core/interface/adjgraph.h \
DetectorDescription/Core/interface/graphwalker.h \
Environment/Configuration/interface/MultiType.h \
FWCore/MessageLogger/interface/Colors.h \
FWCore/MessageLogger/interface/MessageLogger.h \
FWCore/ParameterSet/interface/ParameterSet.h \
FWCore/Utilities/interface/Exception.h \
MagneticField/Engine/interface/MagneticField.h \
RaveTools/Converters/interface/MagneticFieldSingleton.h \
RaveTools/Converters/interface/PropagatorSingleton.h \
DataFormats/GeometrySurface/interface/BlockWipedAllocator.h \
TrackingTools/AnalyticalJacobians/interface/JacobianCartesianToCurvilinear.h \
TrackingTools/AnalyticalJacobians/interface/JacobianCurvilinearToCartesian.h \
TrackingTools/GeomPropagators/interface/HelixBarrelCylinderCrossing.h \
TrackingTools/GeomPropagators/interface/HelixBarrelPlaneCrossingByCircle.h \
TrackingTools/GeomPropagators/interface/HelixForwardPlaneCrossing.h \
TrackingTools/GeomPropagators/interface/HelixPlaneCrossing.h \
TrackingTools/GeomPropagators/interface/Propagator.h \
TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h \
TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h \
TrackingTools/TrajectoryParametrization/interface/CartesianTrajectoryError.h \
TrackingTools/TrajectoryParametrization/interface/CurvilinearTrajectoryError.h \
TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h \
TrackingTools/TrajectoryParametrization/interface/LocalTrajectoryError.h \
TrackingTools/TrajectoryParametrization/interface/LocalTrajectoryParameters.h \
TrackingTools/TrajectoryParametrization/interface/PerigeeTrajectoryError.h \
TrackingTools/TrajectoryParametrization/interface/PerigeeTrajectoryParameters.h \
TrackingTools/TrajectoryParametrization/interface/TrajectoryStateExceptions.h \
TrackingTools/TrajectoryState/interface/BasicTrajectoryState.h \
TrackingTools/TrajectoryState/interface/CopyUsingClone.h \
TrackingTools/TrajectoryState/interface/CopyUsingNew.h \
TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h \
TrackingTools/TrajectoryState/interface/PerigeeConversions.h \
TrackingTools/TrajectoryState/interface/ProxyBase.h \
TrackingTools/TrajectoryState/interface/SurfaceSideDefinition.h \
TrackingTools/TrajectoryState/interface/TrajectoryStateClosestToPoint.h \
TrackingTools/TrajectoryState/interface/TrajectoryStateClosestToBeamLine.h \
TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h \
TrackingTools/TrajectoryState/interface/BasicSingleTrajectoryState.h \
TrackingTools/TransientTrack/interface/TrackTransientTrack.h \
TrackingTools/TransientTrack/interface/TransientTrack.h \
TrackingTools/TransientTrack/interface/BasicTransientTrack.h \
TrackingTools/PatternTools/interface/TrajectoryStateClosestToPointBuilder.h \
FWCore/Utilities/interface/CodedException.h \
FWCore/Utilities/interface/EDMException.h

noinst_HEADERS=\
ROOT/mathcore/TMath.h \
ROOT/genvector/Math/LorentzVector.h \
ROOT/genvector/Math/GenVector/LorentzVector.h \
ROOT/genvector/Math/GenVector/PxPyPzM4D.h \
ROOT/genvector/Math/GenVector/PxPyPzE4D.h \
ROOT/genvector/Math/PxPyPzE4D.h \
ROOT/genvector/Math/GenVector/PtEtaPhiM4D.h \
ROOT/genvector/Math/GenVector/PtEtaPhiE4D.h \
ROOT/genvector/Math/GenVector/BoostX.h \
ROOT/genvector/Math/GenVector/BoostY.h \
ROOT/genvector/Math/GenVector/BoostZ.h \
ROOT/genvector/Math/GenVector/VectorUtil.h \
ROOT/genvector/Math/GenVector/Boost.h \
ROOT/genvector/Math/PtEtaPhiE4D.h \
ROOT/genvector/Math/GenVector/PositionVector3Dfwd.h \
ROOT/genvector/Math/VectorUtil.h \
DataFormats/BTauReco/interface/TrackIPTagInfo.h \
DataFormats/TrackReco/interface/Track.h \
DataFormats/TrackReco/interface/TrackExtraFwd.h \
DataFormats/TrackReco/interface/TrackBase.h \
DataFormats/TrackReco/interface/fillCovariance.h \
DataFormats/JetReco/interface/JetTracksAssociation.h \
DataFormats/GeometryVector/interface/VectorUtil.h \
DataFormats/VertexReco/interface/VertexFwd.h \
CommonTools/Clustering1D/interface/FsmwClusterizer1D.h \
CommonTools/Clustering1D/interface/Clusterizer1D.h \
CommonTools/Clustering1D/interface/Clusterizer1DCommons.h \
CommonTools/Clustering1D/interface/TrivialWeightEstimator.h \
CommonTools/Clustering1D/interface/Clustering1DException.h \
CommonTools/Clustering1D/interface/Cluster1D.h \
CommonTools/Clustering1D/interface/WeightEstimator.h \
CommonTools/Statistics/interface/StatisticsException.h \
CommonTools/Clustering1D/interface/OutermostClusterizer1D.h \
CommonTools/Clustering1D/interface/TrackDistanceWeightEstimator.h \
TrackingTools/AnalyticalJacobians/interface/AnalyticalCurvilinearJacobian.h \
TrackingTools/AnalyticalJacobians/interface/CurvilinearJacobian.h \
TrackingTools/AnalyticalJacobians/interface/JacobianCartesianToLocal.h \
TrackingTools/AnalyticalJacobians/interface/JacobianCurvilinearToLocal.h \
TrackingTools/AnalyticalJacobians/interface/JacobianLocalToCartesian.h \
TrackingTools/AnalyticalJacobians/interface/JacobianLocalToCurvilinear.h \
TrackingTools/GeomPropagators/interface/AnalyticalImpactPointExtrapolator.h \
TrackingTools/GeomPropagators/interface/AnalyticalPropagatorESProducer.h \
TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h \
TrackingTools/GeomPropagators/interface/AnalyticalTrajectoryExtrapolatorToLine.h \
TrackingTools/GeomPropagators/interface/HelixArbitraryPlaneCrossing2Order.h \
TrackingTools/GeomPropagators/interface/HelixArbitraryPlaneCrossing.h \
TrackingTools/GeomPropagators/interface/HelixBarrelPlaneCrossing2OrderLocal.h \
TrackingTools/GeomPropagators/interface/HelixExtrapolatorToLine2Order.h \
TrackingTools/GeomPropagators/interface/HelixLineExtrapolation.h \
TrackingTools/GeomPropagators/interface/IterativeHelixExtrapolatorToLine.h \
TrackingTools/GeomPropagators/interface/PropagationDirectionChooser.h \
TrackingTools/GeomPropagators/interface/PropagationDirectionFromPath.h \
TrackingTools/GeomPropagators/interface/PropagationExceptions.h \
TrackingTools/GeomPropagators/interface/StraightLineBarrelCylinderCrossing.h \
TrackingTools/GeomPropagators/interface/StraightLineCylinderCrossing.h \
TrackingTools/GeomPropagators/interface/StraightLinePlaneCrossing.h \
TrackingTools/GeomPropagators/interface/StraightLinePropagator.h \
TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h \
TrackingTools/PatternTools/interface/ClosestApproachOnHelices.h \
TrackingTools/PatternTools/interface/TwoTrackMinimumDistance.h \
TrackingTools/PatternTools/interface/TwoTrackMinimumDistanceHelixHelix.h \
TrackingTools/PatternTools/interface/TwoTrackMinimumDistanceHelixLine.h \
TrackingTools/PatternTools/interface/TwoTrackMinimumDistanceLineLine.h \
TrackingTools/PatternTools/interface/TrajectoryStateClosestToBeamLineBuilder.h \
TrackingTools/PatternTools/interface/TSCBLBuilderNoMaterial.h \
TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h \
TrackingTools/TransientTrack/interface/TransientTrackBuilder.h  \
TrackingTools/TransientTrack/interface/TransientTrackFromFTSFactory.h \
TrackingTools/TransientTrack/interface/TransientTrackFromFTS.h \
Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h \
DataFormats/GeometrySurface/interface/BoundCone.h \
DataFormats/GeometrySurface/interface/SimpleConeBounds.h \
DataFormats/GeometrySurface/interface/TangentPlane.h \
DataFormats/GeometrySurface/interface/BoundCylinder.h \
DataFormats/GeometrySurface/interface/BoundDisk.h \
DataFormats/GeometrySurface/interface/BoundingBox.h \
DataFormats/GeometrySurface/interface/Cone.h \
DataFormats/GeometrySurface/interface/ConeBuilder.h \
DataFormats/GeometrySurface/interface/CylinderBuilder.h \
DataFormats/GeometrySurface/interface/DiskBuilder.h \
DataFormats/GeometrySurface/interface/GeneralNSurfaceDelimitedBounds.h \
DataFormats/GeometrySurface/interface/GeneralTrapezoidalPlaneBounds.h \
DataFormats/GeometrySurface/interface/GeomExceptions.h \
DataFormats/GeometrySurface/interface/GeometricSorting.h \
DataFormats/GeometrySurface/interface/Line.h \
DataFormats/GeometrySurface/interface/OpenBounds.h \
DataFormats/GeometrySurface/interface/PlaneBuilder.h \
DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h \
DataFormats/GeometrySurface/interface/SimpleCylinderBounds.h \
DataFormats/GeometrySurface/interface/SimpleDiskBounds.h \
DataFormats/GeometrySurface/interface/TrapezoidalPlaneBounds.h \
DataFormats/Common/interface/EDProduct.h  \
DataFormats/Common/interface/Handle.h 

implincludedir = $(includedir)/rave/impl

if HAS_DEVELOPERS
  libRaveCore_la_SOURCES +=\
  CommonTools/Clustering1D/src/DivisiveClusterizer1D.cc \
  CommonTools/Clustering1D/src/MultiClusterizer1D.cc \
  CommonTools/Clustering1D/src/Cluster1DCleaner.cc \
  CommonTools/Clustering1D/src/Cluster1DMerger.cc \
  CommonTools/Clustering1D/src/MtvClusterizer1D.cc 

  noinst_HEADERS +=\
  CommonTools/Clustering1D/interface/DivisiveClusterizer1D.h \
  CommonTools/Clustering1D/interface/MultiClusterizer1D.h \
  CommonTools/Clustering1D/interface/Cluster1DCleaner.h \
  CommonTools/Clustering1D/interface/Cluster1DMerger.h \
  CommonTools/Clustering1D/interface/MtvClusterizer1D.h 
endif

if HAS_GSF
  libRaveCore_la_SOURCES +=\
	TrackingTools/GsfTools/src/BasicMultiTrajectoryState.cc \
	TrackingTools/GsfTools/src/GaussianStateLessWeight.h \
	TrackingTools/GsfTools/src/GaussianSumUtilities1D.cc \
	TrackingTools/GsfTools/src/GsfPropagatorAdapter.cc \
  TrackingTools/TransientTrack/src/GsfTransientTrack.cc \
	TrackingTools/GsfTools/src/GSUtilities.cc \
	TrackingTools/GsfTools/src/MultiGaussianState1D.cc \
	TrackingTools/GsfTools/src/MultiGaussianStateCombiner1D.cc \
	TrackingTools/GsfTools/src/MultiStatePropagation.h \
	TrackingTools/GsfTools/src/MultiStatePropagation.icc \
	TrackingTools/GsfTools/src/MultiTrajectoryStateAssembler.cc \
	TrackingTools/GsfTools/src/MultiTrajectoryStateCombiner.cc \
	TrackingTools/GsfTools/src/MultiTrajectoryStateMode.cc \
	TrackingTools/GsfTools/src/TrajectoryStateLessWeight.h \
  TrackingTools/GsfTools/src/MultiGaussianStateTransform.cc \
	DataFormats/GsfTrackReco/src/GsfComponent5D.cc \
	DataFormats/GsfTrackReco/src/GsfTrack.cc \
	DataFormats/GsfTrackReco/src/GsfTrackExtra.cc

  # TrackingTools/GsfTools/src/MultiTrajectoryStateTransform.cc


  noinst_HEADERS +=\
	TrackingTools/GsfTools/interface/BasicMultiTrajectoryState.h \
	TrackingTools/GsfTools/interface/CloseComponentsMerger.h \
	TrackingTools/GsfTools/interface/CloseComponentsMerger.icc \
	TrackingTools/GsfTools/interface/DistanceBetweenComponents.h \
	TrackingTools/GsfTools/interface/GaussianStateLessWeight.h \
	TrackingTools/GsfTools/interface/GaussianSumUtilities1D.h \
	TrackingTools/GsfTools/interface/GaussianSumUtilities.h \
	TrackingTools/GsfTools/interface/GaussianSumUtilities.icc \
	TrackingTools/GsfTools/interface/GsfMatrixTools.h \
	TrackingTools/GsfTools/interface/GsfPropagatorAdapter.h \
	TrackingTools/GsfTools/interface/GSUtilities.h \
	TrackingTools/GsfTools/interface/KeepingNonZeroWeightsMerger.h \
	TrackingTools/GsfTools/interface/KullbackLeiblerDistance.h \
	TrackingTools/GsfTools/interface/KullbackLeiblerDistance.icc \
	TrackingTools/GsfTools/interface/LargestWeightsStateMerger.h \
	TrackingTools/GsfTools/interface/MahalanobisDistance.h \
	TrackingTools/GsfTools/interface/MultiGaussianState1D.h \
	TrackingTools/GsfTools/interface/MultiGaussianStateAssembler.h \
	TrackingTools/GsfTools/interface/MultiGaussianStateAssembler.icc \
	TrackingTools/GsfTools/interface/MultiGaussianStateCombiner1D.h \
	TrackingTools/GsfTools/interface/MultiGaussianStateCombiner.h \
	TrackingTools/GsfTools/interface/MultiGaussianStateCombiner.icc \
	TrackingTools/GsfTools/interface/MultiGaussianState.h \
	TrackingTools/GsfTools/interface/MultiGaussianState.icc \
	TrackingTools/GsfTools/interface/MultiGaussianStateMerger.h \
	TrackingTools/GsfTools/interface/MultiTrajectoryStateAssembler.h \
	TrackingTools/GsfTools/interface/MultiTrajectoryStateCombiner.h \
	TrackingTools/GsfTools/interface/MultiTrajectoryStateMode.h \
	TrackingTools/GsfTools/interface/RCMultiGaussianState.h \
	TrackingTools/GsfTools/interface/RCSingleGaussianState.h \
	TrackingTools/GsfTools/interface/SingleGaussianState1D.h \
	TrackingTools/GsfTools/interface/SingleGaussianState.h \
	TrackingTools/GsfTools/interface/SingleGaussianState.icc \
  TrackingTools/GsfTools/interface/MultiGaussianStateTransform.h \
  TrackingTools/TransientTrack/interface/GsfTransientTrack.h \
	DataFormats/GsfTrackReco/interface/GsfComponent5D.h \
	DataFormats/GsfTrackReco/interface/GsfTrackExtra.h \
	DataFormats/GsfTrackReco/interface/GsfTrackExtraFwd.h \
	DataFormats/GsfTrackReco/interface/GsfTrackFwd.h \
	DataFormats/GsfTrackReco/interface/GsfTrack.h

#	TrackingTools/GsfTools/interface/MultiTrajectoryStateTransform.h 
endif

# convenience library for MarlinRave
noinst_LTLIBRARIES = libMarlinRaveCore.la

libMarlinRaveCore_la_LDFLAGS = $(libRaveCore_la_LDFLAGS)
libMarlinRaveCore_la_LIBADD = $(libRaveCore_la_LIBADD)
libMarlinRaveCore_la_SOURCES = $(libRaveCore_la_SOURCES)

if IS_WIN32
# COREOBJS=.libs/*.o
COREOBJS=$(libRaveCore_la_OBJECTS:.lo=.o)

dll: libRaveCore.la
	$(CXX) -shared $(libRaveCore_la_LDFLAGS) $(COREOBJS) -o${prefix}/bin/libRaveCore.dll $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) -Wl,--rpath -Wl,${prefix}/bin/

dlls: dll
	cd RecoVertex && ${MAKE} dll
if HAS_FLAVORTAGGING
	cd RecoBTag && ${MAKE} dll
endif
if HAS_KINEMATICS
	cd RecoVertex/KinematicFit && ${MAKE} dll
endif
	cd RaveBase && ${MAKE} dll

endif
