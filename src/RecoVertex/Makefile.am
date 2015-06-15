# JAVAC_FLAGS is needed, to enable -fno-strict-aliasing when interfacing to java.
AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/ROOT/genvector/ -I$(top_srcdir)/src/ROOT/mathcore/ -I$(top_srcdir)/src/ROOT/smatrix/ -I$(top_srcdir)/src/ROOT/ -I$(top_srcdir)/src/RaveBase/RaveInterface -I$(CLHEPINCPATH) $(BOOST_CPPFLAGS) -DRAVE $(JAVAC_FLAGS) $(PLATFORMCPPFLAGS)

SUBDIRS=

# library
lib_LTLIBRARIES = libRaveVertex.la

../libRaveCore.la:
	cd .. && ${MAKE} libRaveCore.la

libRaveVertex_la_LDFLAGS = -L../.libs/ $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) $(PLATFORMLDFLAGS)
libRaveVertex_la_LIBADD = $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) ../libRaveCore.la

libRaveVertex_la_SOURCES = \
VertexTools/src/hsm_3d.cc \
AdaptiveVertexFinder/src/AdaptiveVertexReconstructor.cc \
AdaptiveVertexFit/src/AdaptiveVertexFitter.cc \
ConfigurableVertexReco/src/ConfigurableAdaptiveFitter.cc \
ConfigurableVertexReco/src/ConfigurableAdaptiveReconstructor.cc \
ConfigurableVertexReco/src/ConfigurableAnnealing.cc \
ConfigurableVertexReco/src/ConfigurableKalmanFitter.cc \
ConfigurableVertexReco/src/ConfigurableVertexReconstructor.cc \
ConfigurableVertexReco/src/ConfigurableVertexFitter.cc \
ConfigurableVertexReco/src/ReconstructorFromFitter.cc \
ConfigurableVertexReco/src/VertexRecoManager.cc \
ConfigurableVertexReco/src/VertexFitterManager.cc \
ConfigurableVertexReco/src/AbstractConfFitter.cc \
KalmanVertexFit/src/KalmanSmoothedVertexChi2Estimator.cc \
KalmanVertexFit/src/KalmanTrackToTrackCovCalculator.cc \
KalmanVertexFit/src/KalmanVertexFitter.cc \
KalmanVertexFit/src/KalmanVertexTrackCompatibilityEstimator.cc \
KalmanVertexFit/src/KalmanVertexTrackUpdator.cc \
KalmanVertexFit/src/KalmanVertexSmoother.cc \
KalmanVertexFit/src/KalmanVertexUpdator.cc \
KalmanVertexFit/src/KVFHelper.cc \
LinearizationPointFinders/src/CrossingPtBasedLinearizationPointFinder.cc \
LinearizationPointFinders/src/DefaultLinearizationPointFinder.cc \
LinearizationPointFinders/src/FallbackLinearizationPointFinder.cc \
LinearizationPointFinders/src/FsmwLinearizationPointFinder.cc \
VertexPrimitives/src/BasicSingleVertexState.cc \
VertexPrimitives/src/BasicVertexState.cc \
VertexPrimitives/src/CachingVertex.cc \
VertexPrimitives/src/LinearizedTrackState.cc \
VertexPrimitives/src/TransientVertex.cc \
VertexPrimitives/src/VertexState.cc \
VertexPrimitives/src/VertexTrack.cc \
VertexTools/src/DeterministicAnnealing.cc \
VertexTools/src/DummyTrackToTrackCovCalculator.cc \
VertexTools/src/DummyVertexSmoother.cc \
VertexTools/src/DummyVertexTrackUpdator.cc \
VertexTools/src/FsmwModeFinder3d.cc \
VertexTools/src/GeometricAnnealing.cc \
VertexTools/src/HsmModeFinder3d.cc \
VertexTools/src/LinearizationPointFinder.cc \
VertexTools/src/LinearizedTrackStateFactory.cc \
VertexTools/src/NbSharedTracks.cc \
VertexTools/src/PerigeeLinearizedTrackState.cc \
VertexTools/src/PerigeeRefittedTrackState.cc \
VertexTools/src/SequentialVertexFitter.cc \
VertexTools/src/SequentialVertexSmoother.cc \
VertexTools/src/VertexDistance.cc \
VertexTools/src/VertexDistanceXY.cc \
VertexTools/src/VertexDistance3D.cc 

nobase_implinclude_HEADERS = \
ConfigurableVertexReco/interface/AbstractConfReconstructor.h \
ConfigurableVertexReco/interface/AbstractConfFitter.h \
ConfigurableVertexReco/interface/VertexRecoManager.h \
ConfigurableVertexReco/interface/VertexFitterManager.h \
VertexPrimitives/interface/BasicVertexState.h \
VertexPrimitives/interface/CachingVertex.h \
VertexPrimitives/interface/LinearizedTrackState.h \
VertexPrimitives/interface/RefCountedLinearizedTrackState.h \
VertexPrimitives/interface/RefCountedRefittedTrackState.h \
VertexPrimitives/interface/RefCountedVertexTrack.h \
VertexPrimitives/interface/RefittedTrackState.h \
VertexPrimitives/interface/TrackMap.h \
VertexPrimitives/interface/TrackToTrackMap.h \
VertexPrimitives/interface/TransientVertex.h \
VertexPrimitives/interface/TTtoTTmap.h \
VertexPrimitives/interface/TTtoVtxmap.h \
VertexPrimitives/interface/VertexException.h \
VertexPrimitives/interface/VertexReconstructor.h \
VertexPrimitives/interface/VertexState.h \
VertexPrimitives/interface/VertexTrack.h \
VertexTools/interface/AnnealingSchedule.h \
VertexTools/interface/GeometricAnnealing.h \
VertexTools/interface/DeterministicAnnealing.h

noinst_HEADERS = \
AdaptiveVertexFinder/interface/AdaptiveVertexReconstructor.h \
AdaptiveVertexFit/interface/AdaptiveVertexFitter.h \
ConfigurableVertexReco/interface/ConfigurableAdaptiveFitter.h \
ConfigurableVertexReco/interface/ConfigurableAdaptiveReconstructor.h \
ConfigurableVertexReco/interface/ConfigurableKalmanFitter.h \
ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h \
ConfigurableVertexReco/interface/ConfigurableVertexFitter.h \
ConfigurableVertexReco/interface/ConfRecoBuilder.h \
ConfigurableVertexReco/interface/ConfFitterBuilder.h \
ConfigurableVertexReco/interface/ConfigurableAnnealing.h \
ConfigurableVertexReco/interface/ReconstructorFromFitter.h \
KalmanVertexFit/interface/KalmanSmoothedVertexChi2Estimator.h \
KalmanVertexFit/interface/KalmanTrackToTrackCovCalculator.h \
KalmanVertexFit/interface/KalmanVertexFitter.h \
KalmanVertexFit/interface/KalmanVertexTrackCompatibilityEstimator.h \
KalmanVertexFit/interface/KalmanVertexTrackUpdator.h \
KalmanVertexFit/interface/KalmanVertexUpdator.h \
KalmanVertexFit/interface/KalmanVertexSmoother.h \
KalmanVertexFit/interface/KVFHelper.h \
LinearizationPointFinders/interface/CrossingPtBasedLinearizationPointFinder.h \
LinearizationPointFinders/interface/DefaultLinearizationPointFinder.h \
LinearizationPointFinders/interface/FallbackLinearizationPointFinder.h \
LinearizationPointFinders/interface/FsmwLinearizationPointFinder.h \
VertexPrimitives/interface/BasicSingleVertexState.h \
VertexPrimitives/interface/ConvertError.h \
VertexPrimitives/interface/ConvertToFromReco.h \
VertexPrimitives/interface/TrackToTrackCovCalculator.h \
VertexPrimitives/interface/VertexFitter.h \
VertexPrimitives/interface/VertexSmoothedChiSquaredEstimator.h \
VertexPrimitives/interface/VertexSmoother.h \
VertexPrimitives/interface/VertexTrackCompatibilityEstimator.h \
VertexPrimitives/interface/VertexTrackUpdator.h \
VertexPrimitives/interface/VertexUpdator.h \
VertexTools/interface/hsm_3d.h \
VertexTools/interface/AbstractLTSFactory.h \
VertexTools/interface/DummyTrackToTrackCovCalculator.h \
VertexTools/interface/DummyVertexSmoother.h \
VertexTools/interface/DummyVertexTrackUpdator.h \
VertexTools/interface/FsmwModeFinder3d.h \
VertexTools/interface/LinearizationPointFinder.h \
VertexTools/interface/LinearizedTrackStateFactory.h \
VertexTools/interface/ModeFinder3d.h \
VertexTools/interface/HsmModeFinder3d.h \
VertexTools/interface/NbSharedTracks.h \
VertexTools/interface/PerigeeLinearizedTrackState.h \
VertexTools/interface/PerigeeRefittedTrackState.h \
VertexTools/interface/RecTracksDistanceMatrix.h \
VertexTools/interface/SequentialVertexFitter.h \
VertexTools/interface/SequentialVertexSmoother.h \
VertexTools/interface/VertexDistance3D.h \
VertexTools/interface/VertexDistance.h \
VertexTools/interface/VertexDistanceXY.h \
VertexTools/interface/VertexTrackFactory.h

if HAS_TTF
  libRaveVertex_la_SOURCES += \
  ConfigurableVertexReco/src/ConfigurableTertiaryTracksFinder.cc \
  TertiaryTracksVertexFinder/src/AddTvTrack.cc \
  TertiaryTracksVertexFinder/src/ConfigurableTertiaryTracksVertexFinder.cc \
  TertiaryTracksVertexFinder/src/Flight2DSvFilter.cc \
  TertiaryTracksVertexFinder/src/GetLineCovMatrix.cc \
  TertiaryTracksVertexFinder/src/PvSvFilter.cc \
  TertiaryTracksVertexFinder/src/TertiaryTracksVertexFinder.cc \
  TertiaryTracksVertexFinder/src/DistanceOfVertices2D.cc \
  TertiaryTracksVertexFinder/src/V0SvFilter.cc \
  TertiaryTracksVertexFinder/src/VertexMass.cc

  noinst_HEADERS+=\
  ConfigurableVertexReco/interface/ConfigurableTertiaryTracksFinder.h \
  TertiaryTracksVertexFinder/interface/AddTvTrack.h \
  TertiaryTracksVertexFinder/interface/ConfigurableTertiaryTracksVertexFinder.h \
  TertiaryTracksVertexFinder/interface/DistanceOfVertices2D.h \
  TertiaryTracksVertexFinder/interface/DistanceOfVertices.h \
  TertiaryTracksVertexFinder/interface/Flight2DSvFilter.h \
  TertiaryTracksVertexFinder/interface/GetLineCovMatrix.h \
  TertiaryTracksVertexFinder/interface/PvSvFilter.h \
  TertiaryTracksVertexFinder/interface/TertiaryTracksVertexFinder.h \
  TertiaryTracksVertexFinder/interface/TransientTrackInGroupOfTracks.h \
  TertiaryTracksVertexFinder/interface/TransientTrackInVertices.h \
  TertiaryTracksVertexFinder/interface/V0SvFilter.h \
  TertiaryTracksVertexFinder/interface/VertexMass.h 
endif

if HAS_MVF
  libRaveVertex_la_SOURCES += \
  MultiVertexFit/src/LinPtFinderFromAdaptiveFitter.cc \
  MultiVertexFit/src/LinTrackCache.cc \
  MultiVertexFit/src/MultiVertexBSeeder.cc \
  MultiVertexFit/src/MultiVertexFitter.cc \
  MultiVertexFit/src/MultiVertexReconstructor.cc \
  ConfigurableVertexReco/src/ConfigurableMultiVertexBSeeder.cc \
  ConfigurableVertexReco/src/ConfigurableMultiVertexFitter.cc 

  nobase_implinclude_HEADERS += \
  ConfigurableVertexReco/interface/ConfigurableMultiVertexFitter.h \
  ConfigurableVertexReco/interface/ConfigurableMultiVertexBSeeder.h

  noinst_HEADERS+= \
  MultiVertexFit/interface/LinPtFinderFromAdaptiveFitter.h \
  MultiVertexFit/interface/LinTrackCache.h \
  MultiVertexFit/interface/MultiVertexBSeeder.h \
  MultiVertexFit/interface/MultiVertexFitter.h \
  MultiVertexFit/interface/MultiVertexReconstructor.h 
endif

if HAS_TKVF
  libRaveVertex_la_SOURCES += \
  ConfigurableVertexReco/src/ConfigurableTrimmedKalmanFinder.cc \
  TrimmedKalmanVertexFinder/src/ConfigurableTrimmedVertexFinder.cc \
  TrimmedKalmanVertexFinder/src/KalmanTrimmedVertexFinder.cc \
  TrimmedKalmanVertexFinder/src/TrimmedTrackFilter.cc \
  TrimmedKalmanVertexFinder/src/TrimmedVertexFinder.cc

  noinst_HEADERS+=\
  ConfigurableVertexReco/interface/ConfigurableTrimmedKalmanFinder.h \
  TrimmedKalmanVertexFinder/interface/ConfigurableTrimmedVertexFinder.h \
  TrimmedKalmanVertexFinder/interface/KalmanTrimmedVertexFinder.h \
  TrimmedKalmanVertexFinder/interface/TrimmedTrackFilter.h \
  TrimmedKalmanVertexFinder/interface/TrimmedVertexFinder.h
endif

if HAS_GSF
  libRaveVertex_la_SOURCES += \
  GaussianSumVertexFit/src/AdaptiveGsfVertexFitter.cc \
  GaussianSumVertexFit/src/BasicMultiVertexState.cc \
  GaussianSumVertexFit/src/GsfVertexFitter.cc \
  GaussianSumVertexFit/src/GsfVertexMerger.cc \
  GaussianSumVertexFit/src/GsfVertexSmoother.cc \
  GaussianSumVertexFit/src/GsfVertexTrackCompatibilityEstimator.cc \
  GaussianSumVertexFit/src/GsfVertexUpdator.cc \
  GaussianSumVertexFit/src/GsfVertexWeightCalculator.cc \
  GaussianSumVertexFit/src/MultiPerigeeLTSFactory.cc \
  GaussianSumVertexFit/src/MultiRefittedTS.cc \
  GaussianSumVertexFit/src/MultiVertexStateCombiner.cc \
  GaussianSumVertexFit/src/PerigeeMultiLTS.cc \
  GaussianSumVertexFit/src/VertexGaussianStateConversions.cc \
  ConfigurableVertexReco/src/ConfigurableAdaptiveGaussianSumFilter.cc \
  ConfigurableVertexReco/src/ConfigurableGaussianSumFilter.cc

  noinst_HEADERS+=\
  GaussianSumVertexFit/interface/AdaptiveGsfVertexFitter.h \
  GaussianSumVertexFit/interface/BasicMultiVertexState.h \
  GaussianSumVertexFit/interface/GsfVertexFitter.h \
  GaussianSumVertexFit/interface/GsfVertexMerger.h \
  GaussianSumVertexFit/interface/GsfVertexSmoother.h \
  GaussianSumVertexFit/interface/GsfVertexTrackCompatibilityEstimator.h \
  GaussianSumVertexFit/interface/GsfVertexUpdator.h \
  GaussianSumVertexFit/interface/GsfVertexWeightCalculator.h \
  GaussianSumVertexFit/interface/MultiPerigeeLTSFactory.h \
  GaussianSumVertexFit/interface/MultiRefittedTS.h \
  GaussianSumVertexFit/interface/MultiVertexStateCombiner.h \
  GaussianSumVertexFit/interface/PerigeeMultiLTS.h \
  GaussianSumVertexFit/interface/VertexGaussianStateConversions.h \
  ConfigurableVertexReco/interface/ConfigurableAdaptiveGaussianSumFilter.h \
  ConfigurableVertexReco/interface/ConfigurableGaussianSumFilter.h
endif


if HAS_ZVRES
  AM_CPPFLAGS += -I$(top_srcdir)/src/RecoVertex/ZvresReco/src/vertex_lcfi/algo/ \
				 -I$(top_srcdir)/src/RecoVertex/ZvresReco/src/vertex_lcfi/
  libRaveVertex_la_SOURCES += \
  ZvresReco/src/ZvresVertexReconstructor.cc \
  ZvresReco/src/vertex_lcfi/algo/src/pereventipfitter.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/vertexmomentum.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/vertexmass.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/zvkin.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/paramsignificance.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/jointprob.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/vertexmultiplicity.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/vertexcharge.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/decaysignificance.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/trackattach.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/zvres.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/twotrackpid.cpp \
  ZvresReco/src/vertex_lcfi/algo/src/secondvertexprob.cpp \
  ZvresReco/src/vertex_lcfi/algo/inc/trackattach.h \
  ZvresReco/src/vertex_lcfi/algo/inc/zvres.h \
  ZvresReco/src/vertex_lcfi/algo/inc/twotrackpid.h \
  ZvresReco/src/vertex_lcfi/algo/inc/secondvertexprob.h \
  ZvresReco/src/vertex_lcfi/algo/inc/pereventipfitter.h \
  ZvresReco/src/vertex_lcfi/algo/inc/vertexmomentum.h \
  ZvresReco/src/vertex_lcfi/algo/inc/vertexmass.h \
  ZvresReco/src/vertex_lcfi/algo/inc/zvkin.h \
  ZvresReco/src/vertex_lcfi/algo/inc/paramsignificance.h \
  ZvresReco/src/vertex_lcfi/algo/inc/jointprob.h \
  ZvresReco/src/vertex_lcfi/algo/inc/vertexmultiplicity.h \
  ZvresReco/src/vertex_lcfi/algo/inc/vertexcharge.h \
  ZvresReco/src/vertex_lcfi/algo/inc/decaysignificance.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/interactionpoint.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexfitter.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexfuncmaxfinderclassicstepper.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexfunctionelement.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexfunctionsimple.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexresolver.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/gausstube.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/levmarminimiser.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/doc.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexresolverequalsteps.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexfitterlsm.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexfinderclassic.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexfinderghost.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/candidatevertex.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/maxminfinder.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/ghostfinderstage1.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexfunction.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexfunctionclassic.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/vertexfuncmaxfinder.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/gaussellipsoid.h \
  ZvresReco/src/vertex_lcfi/zvtop/include/VertexFitterKalman.h \
  ZvresReco/src/vertex_lcfi/zvtop/src/VertexFitterKalman.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/ghostfinderstage1.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/gausstube.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/vertexfunctionclassic.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/vertexresolverequalsteps.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/gaussellipsoid.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/vertexfinderclassic.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/vertexfitterlsm.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/interactionpoint.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/candidatevertex.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/vertexfinderghost.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/vertexfuncmaxfinderclassicstepper.cpp \
  ZvresReco/src/vertex_lcfi/zvtop/src/vertexfunctionsimple.cpp \
  ZvresReco/src/vertex_lcfi/src/event.cpp \
  ZvresReco/src/vertex_lcfi/src/jet.cpp \
  ZvresReco/src/vertex_lcfi/src/track.cpp \
  ZvresReco/src/vertex_lcfi/src/trackstate.cpp \
  ZvresReco/src/vertex_lcfi/src/decaychain.cpp \
  ZvresReco/src/vertex_lcfi/src/vertex.cpp \
  ZvresReco/src/vertex_lcfi/src/TState.cpp \
  ZvresReco/src/vertex_lcfi/inc/TState.h \
  ZvresReco/src/vertex_lcfi/inc/template.h \
  ZvresReco/src/vertex_lcfi/inc/vertex.h \
  ZvresReco/src/vertex_lcfi/inc/event.h \
  ZvresReco/src/vertex_lcfi/inc/algo.h \
  ZvresReco/src/vertex_lcfi/inc/lcio.h \
  ZvresReco/src/vertex_lcfi/inc/jet.h \
  ZvresReco/src/vertex_lcfi/inc/track.h \
  ZvresReco/src/vertex_lcfi/inc/doc.h \
  ZvresReco/src/vertex_lcfi/inc/trackstate.h \
  ZvresReco/src/vertex_lcfi/inc/decaychain.h \
  ZvresReco/src/vertex_lcfi/util/src/memorymanager.cpp \
  ZvresReco/src/vertex_lcfi/util/src/matrix.cpp \
  ZvresReco/src/vertex_lcfi/util/src/vector3.cpp \
  ZvresReco/src/vertex_lcfi/util/src/string.cpp \
  ZvresReco/src/vertex_lcfi/util/src/util.cpp \
  ZvresReco/src/vertex_lcfi/util/inc/projection.h \
  ZvresReco/src/vertex_lcfi/util/inc/util.h \
  ZvresReco/src/vertex_lcfi/util/inc/helixrep.h \
  ZvresReco/src/vertex_lcfi/util/inc/memorymanager.h \
  ZvresReco/src/vertex_lcfi/util/inc/matrix.h \
  ZvresReco/src/vertex_lcfi/util/inc/recalculate.h \
  ZvresReco/src/vertex_lcfi/util/inc/doc.h \
  ZvresReco/src/vertex_lcfi/util/inc/vector3.h \
  ZvresReco/src/vertex_lcfi/util/inc/string.h \
  ConfigurableVertexReco/src/ConfigurableZvresReconstructor.cc

  noinst_HEADERS+=\
  ZvresReco/interface/ZvresVertexReconstructor.h \
  ConfigurableVertexReco/interface/ConfigurableZvresReconstructor.h 
  
# for debugging only
# nobase_implinclude_HEADERS = 
endif

if HAS_DEVELOPERS
  libRaveVertex_la_SOURCES += \
  LinearizationPointFinders/src/LMSLinearizationPointFinder.cc \
  LinearizationPointFinders/src/MonteCarloLinearizationPointFinder.cc \
  LinearizationPointFinders/src/SMSLinearizationPointFinder.cc \
  LinearizationPointFinders/src/SubsetHSMLinearizationPointFinder.cc \
  LinearizationPointFinders/src/ZeroLinearizationPointFinder.cc \
  LinearizationPointFinders/src/GenericLinearizationPointFinder.cc \
  LinearizationPointFinders/src/HSMLinearizationPointFinder.cc \
  ConfigurableVertexReco/src/ConfigurableLinPtFinder.cc \
  ConfigurableVertexReco/src/FitterFromLinPtFinder.cc \
  VertexTools/src/lms_3d.cc \
  VertexTools/src/Lms3d.cc \
  VertexTools/src/LmsModeFinder3d.cc \
  VertexTools/src/SMS.cc \
  VertexTools/src/SmsModeFinder3d.cc \
  VertexTools/src/SubsetHsmModeFinder3d.cc 

  noinst_HEADERS += \
  LinearizationPointFinders/interface/MonteCarloLinearizationPointFinder.h \
  LinearizationPointFinders/interface/ZeroLinearizationPointFinder.h \
  LinearizationPointFinders/interface/HSMLinearizationPointFinder.h \
  LinearizationPointFinders/interface/LMSLinearizationPointFinder.h \
  LinearizationPointFinders/interface/SMSLinearizationPointFinder.h \
  LinearizationPointFinders/interface/GenericLinearizationPointFinder.h \
  LinearizationPointFinders/interface/SubsetHSMLinearizationPointFinder.h \
  ConfigurableVertexReco/interface/ConfigurableLinPtFinder.h \
  ConfigurableVertexReco/interface/FitterFromLinPtFinder.h \
  VertexTools/interface/lms_3d.h \
  VertexTools/interface/Lms3d.h \
  VertexTools/interface/LmsModeFinder3d.h \
  VertexTools/interface/SMS.h \
  VertexTools/interface/SmsModeFinder3d.h \
  VertexTools/interface/SubsetHsmModeFinder3d.h
endif

if HAS_KINEMATICS
  SUBDIRS += KinematicFit
endif

implincludedir = $(includedir)/rave/impl/RecoVertex

if IS_WIN32
dll: $(libRaveVertex_la_OBJECTS) $(libRaveVertex_la_DEPENDENCIES)
#	$(LIBTOOL) --tag=CXX --mode=link $(CXX) $(PLATFORMLDFLAGS) -l../.libs/libRaveCore.exe -o .libs/libRaveVertex -rpath $(prefix)/bin/ $(libRaveVertex_la_OBJECTS)
	$(CXX) -shared -L${prefix}/bin $(libRaveVertex_la_LDFLAGS) $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) .libs/*.o -o ${prefix}/bin/libRaveVertex.dll -lRaveCore -Wl,--rpath -Wl,${prefix}/bin/
#  -Wl,--enable-auto-import -Wl,--export-all-symbols 

install_dll:
	$(install_sh) .libs/libRaveVertex.dll ${prefix}/bin/
endif

# You will need to add the .cc files to
# libRaveVertex_la_SOURCES, and the .h files to
# noinst_HEADERS,
# if you want them to be built!
# (plus renaming them wouldnt be a wrong thing to do, either)
 
EXTRA_DIST = \
SkeletonVertexReco/src/SkeletonVertexReconstructor.cc \
ConfigurableVertexReco/src/ConfigurableSkeletonReconstructor.cc \
SkeletonVertexReco/interface/SkeletonVertexReconstructor.h \
ConfigurableVertexReco/interface/ConfigurableSkeletonReconstructor.h
