# JAVAC_FLAGS is needed, to enable -fno-strict-aliasing when interfacing to java.
AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/ROOT/genvector/ -I$(top_srcdir)/src/ROOT/mathcore/ -I$(top_srcdir)/src/ROOT/smatrix/ -I$(top_srcdir)/src/ROOT/ -I$(top_srcdir)/src/RaveBase/RaveInterface -I$(CLHEPINCPATH) $(BOOST_CPPFLAGS) -DRAVE $(JAVAC_FLAGS) $(PLATFORMCPPFLAGS)

# library
lib_LTLIBRARIES = libRaveVertexKinematics.la

libRaveVertexKinematics_la_LDFLAGS = $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) $(PLATFORMLDFLAGS)
### libRaveVertexKinematics_la_LIBADD = 

libRaveVertexKinematics_la_SOURCES = \
	src/BackToBackKinematicConstraint.cc \
	src/ConstrainedTreeBuilder.cc \
	src/FinalTreeBuilder.cc \
	src/FourMomentumKinematicConstraint.cc \
	src/EqualMassKinematicConstraint.cc \
	src/InputSort.cc \
	src/KinematicConstrainedVertexFitter.cc \
	src/KinematicConstrainedVertexUpdator.cc \
	src/KinematicParticleFitter.cc \
	src/KinematicParticleVertexFitter.cc \
	src/LagrangeChildUpdator.cc \
	src/LagrangeParentParticleFitter.cc \
	src/LagrangeEnergyParentParticleFitter.cc \
	src/MassKinematicConstraint.cc \
	src/MomentumKinematicConstraint.cc \
	src/PointingKinematicConstraint.cc \
	src/SimplePointingConstraint.cc \
	src/SmartPointingConstraint.cc \
	src/TwoTrackMassKinematicConstraint.cc \
	src/VertexKinematicConstraint.cc \
	src/MultiTrackMassKinematicConstraint.cc \
	src/MultiTrackVertexLinkKinematicConstraint.cc \
	src/MultiTrackPointingKinematicConstraint.cc \
	../KinematicFitPrimitives/src/KinematicParameters.cc \
	../KinematicFitPrimitives/src/KinematicParametersError.cc \
	../KinematicFitPrimitives/src/KinematicParticle.cc \
	../KinematicFitPrimitives/src/KinematicParticleFactoryFromTransientTrack.cc \
	../KinematicFitPrimitives/src/KinematicPerigeeConversions.cc \
	../KinematicFitPrimitives/src/KinematicRefittedTrackState.cc \
	../KinematicFitPrimitives/src/KinematicState.cc \
	../KinematicFitPrimitives/src/KinematicTree.cc \
	../KinematicFitPrimitives/src/KinematicVertex.cc \
	../KinematicFitPrimitives/src/KinematicVertexFactory.cc \
	../KinematicFitPrimitives/src/MultipleKinematicConstraint.cc \
	../KinematicFitPrimitives/src/ParticleKinematicLinearizedTrackState.cc \
	../KinematicFitPrimitives/src/ParticleKinematicLinearizedTrackStateFactory.cc \
	../KinematicFitPrimitives/src/PerigeeKinematicState.cc \
	../KinematicFitPrimitives/src/TrackKinematicStatePropagator.cc \
	../KinematicFitPrimitives/src/TransientTrackKinematicParticle.cc \
	../KinematicFitPrimitives/src/TransientTrackKinematicStateBuilder.cc \
	../KinematicFitPrimitives/src/VirtualKinematicParticle.cc \
	../KinematicFitPrimitives/src/VirtualKinematicParticleFactory.cc
#	plugins/KineExample.cc

nobase_implinclude_HEADERS = \
  ../KinematicFitPrimitives/interface/ExtendedPerigeeTrajectoryError.h \
  ../KinematicFitPrimitives/interface/ExtendedPerigeeTrajectoryParameters.h \
  ../KinematicFitPrimitives/interface/KinematicConstraint.h \
  ../KinematicFitPrimitives/interface/KinematicParameters.h \
  ../KinematicFitPrimitives/interface/KinematicParametersError.h \
  ../KinematicFitPrimitives/interface/KinematicParticle.h \
  ../KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h \
  ../KinematicFitPrimitives/interface/KinematicState.h \
  ../KinematicFitPrimitives/interface/KinematicStatePropagator.h \
  ../KinematicFitPrimitives/interface/KinematicTree.h \
  ../KinematicFitPrimitives/interface/KinematicVertex.h \
  ../KinematicFitPrimitives/interface/MultipleKinematicConstraint.h \
  ../KinematicFitPrimitives/interface/ParticleKinematicLinearizedTrackState.h \
  ../KinematicFitPrimitives/interface/ParticleKinematicLinearizedTrackStateFactory.h \
  ../KinematicFitPrimitives/interface/ParticleMass.h \
  ../KinematicFitPrimitives/interface/Matrices.h \
  ../KinematicFitPrimitives/interface/PerigeeKinematicState.h \
  ../KinematicFitPrimitives/interface/RefCountedKinematicParticle.h \
  ../KinematicFitPrimitives/interface/RefCountedKinematicTree.h \
  ../KinematicFitPrimitives/interface/RefCountedKinematicVertex.h \
  ../KinematicFitPrimitives/interface/TrackKinematicStatePropagator.h \
  ../KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h \
  ../KinematicFitPrimitives/interface/TransientTrackKinematicStateBuilder.h \
  ../KinematicFitPrimitives/interface/VirtualKinematicParticle.h

noinst_HEADERS = \
  interface/BackToBackKinematicConstraint.h \
  interface/ChildUpdator.h \
  interface/ConstrainedTreeBuilder.h \
  interface/FinalTreeBuilder.h \
  interface/FourMomentumKinematicConstraint.h \
  interface/EqualMassKinematicConstraint.h \
  interface/InputSort.h \
  interface/KinematicConstrainedVertexFitter.h \
  interface/KinematicConstrainedVertexUpdator.h \
  interface/KinematicParticleFitter.h \
  interface/KinematicParticleVertexFitter.h \
  interface/LagrangeChildUpdator.h \
  interface/LagrangeParentParticleFitter.h \
  interface/LagrangeEnergyParentParticleFitter.h \
  interface/MassKinematicConstraint.h \
  interface/MomentumKinematicConstraint.h \
  interface/ParentParticleFitter.h \
  interface/PointingKinematicConstraint.h \
  interface/SimplePointingConstraint.h \
  interface/SmartPointingConstraint.h \
  interface/TwoTrackMassKinematicConstraint.h \
  interface/VertexKinematicConstraint.h \
  interface/MultiTrackMassKinematicConstraint.h \
  interface/MultiTrackVertexLinkKinematicConstraint.h \
  interface/MultiTrackPointingKinematicConstraint.h \
  ../KinematicFitPrimitives/interface/KinematicPerigeeConversions.h \
  ../KinematicFitPrimitives/interface/KinematicRefittedTrackState.h \
  ../KinematicFitPrimitives/interface/KinematicVertexAssociator.h \
  ../KinematicFitPrimitives/interface/KinematicVertexDistance.h \
  ../KinematicFitPrimitives/interface/KinematicVertexFactory.h \
  ../KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h \
  ../KinematicFitPrimitives/interface/VirtualKinematicParticleFactory.h
#	plugins/KineExample.h

implincludedir = $(includedir)/rave/impl/RecoVertex/KinematicFit

if IS_WIN32
dll: $(libRaveVertexKinematics_la_OBJECTS) $(libRaveVertexKinematics_la_DEPENDENCIES)
#	$(LIBTOOL) --tag=CXX --mode=link $(CXX) $(PLATFORMLDFLAGS) -l../.libs/libRaveCore.exe -o .libs/libRaveVertex -rpath $(prefix)/bin/ $(libRaveVertex_la_OBJECTS)
	$(CXX) -shared -L${prefix}/bin $(libRaveVertexKinematics_la_LDFLAGS) $(CLHEPLIBPATH) .libs/*.o -o ${prefix}/bin/libRaveVertexKinematics.dll -lRaveCore -lRaveVertex -Wl,--rpath -Wl,${prefix}/bin/
#  -Wl,--enable-auto-import -Wl,--export-all-symbols 

install_dll:
	$(install_sh) .libs/libRaveVertexKinematics.dll ${prefix}/bin/
endif

