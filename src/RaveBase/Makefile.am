# JAVAC_FLAGS is needed, to enable -fno-strict-aliasing when interfacing to java.
AM_CPPFLAGS = -ansi -I$(top_srcdir)/src/ -I$(top_srcdir)/src/ROOT/genvector/ -I$(top_srcdir)/src/ROOT/mathcore/ -I$(top_srcdir)/src/ROOT/smatrix/ -I$(top_srcdir)/src/ROOT/ -I$(top_srcdir)/src/RaveBase/RaveInterface -I$(CLHEPINCPATH) $(BOOST_CPPFLAGS) -DRAVE $(JAVAC_FLAGS) $(PLATFORMCPPFLAGS) $(FLAVORTAGGING_CPPFLAGS) $(KINEMATICS_CPPFLAGS)

ADD_FLAVOR_LIB=
ADD_KINEMATICS_LIB=

../RecoVertex/libRaveVertex.la:
	cd ../RecoVertex && ${MAKE} libRaveVertex.la

../RecoVertex/KinematicFit/libRaveVertexKinematics.la:
	cd ../RecoVertex/KinematicFit && ${MAKE} libRaveVertexKinematics.la

../libRaveCore.la:
	cd .. && ${MAKE} libRaveCore.la

../RecoBTag/libRaveFlavorTag.la:
	cd ../RecoBTag && $(MAKE) libRaveFlavorTag.la

# library
lib_LTLIBRARIES = libRaveBase.la

FLAVOR_LA=
if HAS_FLAVORTAGGING
  FLAVOR_LA+=../RecoBTag/libRaveFlavorTag.la
endif

if HAS_GSF
  AM_CPPFLAGS += -DHAS_GSF
endif     

KINEMATICS_LA=
if HAS_KINEMATICS
  KINEMATICS_LA+=../RecoVertex/KinematicFit/libRaveVertexKinematics.la
endif

libRaveBase_la_LDFLAGS = $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) $(PLATFORMLDFLAGS)
libRaveBase_la_LIBADD = ../RecoVertex/libRaveVertex.la ../libRaveCore.la $(FLAVOR_LA) $(KINEMATICS_LA)

libRaveBase_la_SOURCES = \
Converters/src/CmsToRaveObjects.cc \
Converters/src/MagneticFieldWrapper.cc \
Converters/src/PerigeeToRaveObjects.cc \
Converters/src/PropagatorWrapper.cc \
Converters/src/RaveStreamers.cc \
Converters/src/RaveToAlgebraicObjects.cc \
Converters/src/RaveToCmsObjects.cc \
Converters/src/RaveToPerigeeObjects.cc \
RaveEngine/src/RaveId.cc \
RaveEngine/src/RaveVertexReconstructor.cc \
RaveEngine/src/RaveParameterSetBuilder.cc \
RaveEngine/src/RaveBeamSpotSingleton.cc \
RaveInterface/src/BasicTrack.cc \
RaveInterface/src/BasicVertex.cc \
RaveInterface/src/CheckedFloat.cc \
RaveInterface/src/ConstantMagneticField.cc \
RaveInterface/src/Covariance3D.cc \
RaveInterface/src/Covariance33D.cc \
RaveInterface/src/Covariance6D.cc \
RaveInterface/src/Ellipsoid3D.cc \
RaveInterface/src/Exception.cc \
RaveInterface/src/PerigeeParameters3D.cc \
RaveInterface/src/PerigeeParameters5D.cc \
RaveInterface/src/PerigeeCovariance3D.cc \
RaveInterface/src/PerigeeCovariance5D.cc \
RaveInterface/src/Point3D.cc \
RaveInterface/src/Track.cc \
RaveInterface/src/VacuumPropagator.cc \
RaveInterface/src/Vector6D.cc \
RaveInterface/src/Version.cc \
RaveInterface/src/Vertex.cc \
RaveInterface/src/VertexFactory.cc \
Utilities/src/StringTools.cc

pkginclude_HEADERS=\
RaveInterface/rave/AbstractTrack.h \
RaveInterface/rave/AbstractPerigeeAccess.h \
RaveInterface/rave/AbstractVertex.h \
RaveInterface/rave/BasicTrack.h \
RaveInterface/rave/BasicVertex.h \
RaveInterface/rave/Charge.h \
RaveInterface/rave/CheckedFloat.h \
RaveInterface/rave/ConstantMagneticField.h \
RaveInterface/rave/CopyUsingNew.h \
RaveInterface/rave/Covariance3D.h \
RaveInterface/rave/Covariance33D.h \
RaveInterface/rave/Covariance6D.h \
RaveInterface/rave/Ellipsoid3D.h \
RaveInterface/rave/Exception.h \
RaveInterface/rave/MagneticField.h \
RaveInterface/rave/PerigeeParameters3D.h \
RaveInterface/rave/PerigeeParameters5D.h \
RaveInterface/rave/PerigeeCovariance3D.h \
RaveInterface/rave/PerigeeCovariance5D.h \
RaveInterface/rave/Point3D.h \
RaveInterface/rave/Propagator.h \
RaveInterface/rave/ProxyBase.h \
RaveInterface/rave/ReferenceCounted.h \
RaveInterface/rave/Track.h \
RaveInterface/rave/Units.h \
RaveInterface/rave/VacuumPropagator.h \
RaveInterface/rave/Vector3D.h \
RaveInterface/rave/Vector6D.h \
RaveInterface/rave/Version.h \
RaveInterface/rave/Vertex.h \
RaveInterface/rave/VertexFactory.h

noinst_HEADERS = \
Converters/interface/PropagatorWrapper.h

nobase_implinclude_HEADERS=\
Converters/interface/CmsToRaveObjects.h \
Converters/interface/MagneticFieldWrapper.h \
Converters/interface/PerigeeToRaveObjects.h \
Converters/interface/RaveStreamers.h \
Converters/interface/RaveToAlgebraicObjects.h \
Converters/interface/RaveToCmsObjects.h \
Converters/interface/RaveToPerigeeObjects.h \
RaveEngine/interface/RaveBeamSpotSingleton.h \
RaveEngine/interface/RaveId.h \
RaveEngine/interface/RaveVertexReconstructor.h \
RaveEngine/interface/RaveParameterSetBuilder.h \
Utilities/interface/StringTools.h

if HAS_KINEMATICS 
  libRaveBase_la_SOURCES += \
  RaveEngine/src/TaggedTransientTrackKinematicParticle.cc \
  RaveInterface/src/Vector4D.cc \
  RaveInterface/src/Vector7D.cc \
  RaveInterface/src/Covariance4D.cc \
  RaveInterface/src/Covariance7D.cc \
  RaveInterface/src/PerigeeParameters6D.cc \
  RaveInterface/src/PerigeeCovariance6D.cc \
  RaveInterface/src/BasicKinematicConstraint.cc \
  RaveInterface/src/BasicKinematicParticle.cc \
  RaveInterface/src/BasicKinematicTree.cc \
  RaveInterface/src/BasicKinematicVertex.cc \
  RaveInterface/src/BasicMultipleKinematicConstraint.cc \
  RaveInterface/src/BasicTransientTrackKinematicParticle.cc \
  RaveInterface/src/BasicVirtualKinematicParticle.cc \
  RaveInterface/src/KinematicConstraint.cc \
  RaveInterface/src/KinematicParticle.cc \
  RaveInterface/src/KinematicTree.cc \
  RaveInterface/src/Logger.cc \
  RaveInterface/src/KinematicVertex.cc \
  RaveInterface/src/MultipleKinematicConstraint.cc \
  RaveInterface/src/TransientTrackKinematicParticle.cc \
  RaveInterface/src/VirtualKinematicParticle.cc \
  RaveInterface/src/Plane.cc \
  RaveInterface/src/Cylinder.cc \
  RaveInterface/src/KinematicConstraintBuilder.cc \
  RaveInterface/src/KinematicTreeFactory.cc

  pkginclude_HEADERS += \
  RaveInterface/rave/Vector4D.h \
  RaveInterface/rave/Vector7D.h \
  RaveInterface/rave/Covariance4D.h \
  RaveInterface/rave/Covariance7D.h \
  RaveInterface/rave/PerigeeParameters6D.h \
  RaveInterface/rave/PerigeeCovariance6D.h \
  RaveInterface/rave/BasicKinematicConstraint.h \
  RaveInterface/rave/BasicKinematicParticle.h \
  RaveInterface/rave/BasicKinematicTree.h \
  RaveInterface/rave/BasicKinematicVertex.h \
  RaveInterface/rave/BasicMultipleKinematicConstraint.h \
  RaveInterface/rave/BasicTransientTrackKinematicParticle.h \
  RaveInterface/rave/BasicVirtualKinematicParticle.h \
  RaveInterface/rave/KinematicConstraint.h \
  RaveInterface/rave/KinematicParticle.h \
  RaveInterface/rave/KinematicTree.h \
  RaveInterface/rave/Plane.h \
  RaveInterface/rave/Cylinder.h \
  RaveInterface/rave/KinematicVertex.h \
  RaveInterface/rave/Logger.h \
  RaveInterface/rave/MultipleKinematicConstraint.h \
  RaveInterface/rave/TransientTrackKinematicParticle.h \
  RaveInterface/rave/VirtualKinematicParticle.h \
  RaveInterface/rave/KinematicConstraintBuilder.h \
  RaveInterface/rave/KinematicTreeFactory.h

  noinst_HEADERS += \
  RaveEngine/interface/TaggedTransientTrackKinematicParticle.h

  ADD_KINEMATICS_LIB += -lRaveVertexKinematics
endif


if HAS_FLAVORTAGGING
libRaveBase_la_SOURCES += RaveInterface/src/FlavorTagFactory.cc 

pkginclude_HEADERS += RaveInterface/rave/FlavorTagFactory.h

ADD_FLAVOR_LIB +=-lRaveFlavorTag
endif

implincludedir = $(includedir)/rave/impl/RaveBase

if IS_WIN32
dll: libRaveBase.la
	$(CXX) -shared -L${prefix}/bin $(libRaveBase_la_LDFLAGS) $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) .libs/*.o -o ${prefix}/bin/libRaveBase.dll -lRaveVertex -lRaveCore $(ADD_FLAVOR_LIB) $(ADD_KINEMATICS_LIB) -Wl,--rpath -Wl,$(prefix)/bin/
endif
