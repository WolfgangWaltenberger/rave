JAVA_PATH = at/hephy/Rave/
JDK_INCLUDES = -I@JDK_INCLUDE@/include/ -I@JDK_INCLUDE@/include/$(HOST_INCLUDE) -I$(JAVA_HOME)/include/ -I$(JAVA_HOME)/include/$(HOST_INCLUDE) -I$(JAVAROOT)/include/ -I$(JAVAROOT)/include/$(HOST_INCLUDE) -I$(top_builddir)
BUILT_SOURCES = rave.cc
SWIG_SOURCES = $(top_builddir)/rave_autoconf.i
# JAVAOPTS = -java -package at.hephy.Rave -outdir $(JAVA_PATH) -java
JAVAOPTS = -c++ -package at.hephy.Rave -outdir $(JAVA_PATH) -java
SWIGOPTS = -v -c++ -Wall -dirprot -makedefault -I$(top_srcdir)/src/RaveBase/RaveInterface -I$(top_srcdir)/src/ -I$(top_srcdir) $(PLATFORMCPPFLAGS) # -directors
RAVEINCDIR=$(top_srcdir)/src/RaveBase/RaveInterface/rave
# JAVACFLAGS=-d .
CLASSPATH = . #$(srcdir)/$(JAVA_PATH)
# JAVAROOT=$(srcdir)

javalibdir = $(prefix)/lib/jni
javasharedir = $(prefix)/share/java

javalib_LTLIBRARIES=libJavaRave.la
SUBDIRS=tests

../src/RaveBase/libRaveBase.la:
	cd ../src/RaveBase && make libRaveBase.la
../src/RecoVertex/libRaveVertex.la:
	cd ../src/RecoVertex && make libRaveVertex.la
../src/RecoBTag/libRaveFlavorTag.la:
	cd ../src/RecoBTag && make libRaveFlavorTag.la
../src/libRaveCore.la:
	cd ../src/ && make libRaveCore.la

libJavaRave_la_SOURCES = $(BUILT_SOURCES) $(SWIG_SOURCES)
libJavaRave_la_CPPFLAGS = $(PLATFORMCPPFLAGS) $(JDK_INCLUDES) $(SWIG_JAVA_CPPFLAGS) -I$(top_srcdir)/src/RaveBase/RaveInterface/ -I$(top_srcdir)/src/ -Wno-long-long -I$(srcdir) $(JAVAC_FLAGS) $(BOOST_CPPFLAGS)
libJavaRave_la_LDFLAGS = -module $(PLATFORMLDFLAGS)
libJavaRave_la_LIBADD = ../src/RaveBase/libRaveBase.la ../src/libRaveCore.la ../src/RecoVertex/libRaveVertex.la

vars:
	echo "$(top_srcdir)"
	echo "$(JAVAROOT)"
	echo "$(JAVA_HOME)"
	echo "$(JDK_INCLUDE)"
	echo "$(PLATFORMCPPFLAGS)"


JAVA_SOURCE = \
  $(JAVA_PATH)/RaveConstantMagneticField.java \
  $(JAVA_PATH)/RaveCovariance3D.java \
  $(JAVA_PATH)/RaveCovariance6D.java \
  $(JAVA_PATH)/RaveEllipsoid3D.java \
  $(JAVA_PATH)/RaveException.java \
  $(JAVA_PATH)/RaveMagneticField.java \
  $(JAVA_PATH)/RavePoint3D.java \
  $(JAVA_PATH)/RavePropagator.java \
  $(JAVA_PATH)/RaveTrack.java \
  $(JAVA_PATH)/RaveTrackContainer.java \
  $(JAVA_PATH)/RaveVacuumPropagator.java \
  $(JAVA_PATH)/RaveVector6D.java \
  $(JAVA_PATH)/RaveVertex.java \
  $(JAVA_PATH)/RaveCylinder.java \
  $(JAVA_PATH)/RavePlane.java \
  $(JAVA_PATH)/RaveVertexContainer.java \
  $(JAVA_PATH)/RaveVertexFactory.java \
  $(JAVA_PATH)/RavePerigeeCovariance3D.java \
  $(JAVA_PATH)/RavePerigeeCovariance5D.java \
  $(JAVA_PATH)/RavePerigeeParameters5D.java \
  $(JAVA_PATH)/RavePerigeeParameters3D.java \
  $(JAVA_PATH)/SWIGTYPE_p_rave__BasicVertex.java \
  $(JAVA_PATH)/SWIGTYPE_p_rave__VertexReconstructor.java \
  $(JAVA_PATH)/SWIGTYPE_p_std__vectorT_std__pairT_float_rave__Vertex_t_t.java \
  $(JAVA_PATH)/SWIGTYPE_p_std__pairT_rave__Track_double_t.java \
  $(JAVA_PATH)/SWIGTYPE_p_rave__BasicTrack.java \
  $(JAVA_PATH)/SWIGTYPE_p_void.java  \
  $(JAVA_PATH)/WeightedRaveTrack.java \
  $(JAVA_PATH)/WeightedRaveTrackContainer.java \
  $(JAVA_PATH)/rave.java \
  $(JAVA_PATH)/raveJNI.java

#  $(JAVA_PATH)/WeightedRaveTrack.java
#  $(JAVA_PATH)/WeightedRaveTrackContainer.java

if HAS_FLAVORTAGGING
  libJavaRave_la_LIBADD += ../src/RecoBTag/libRaveFlavorTag.la
  libJavaRave_la_CPPFLAGS += -DWITH_FLAVORTAGGING
  SWIGOPTS+=-DWITH_FLAVORTAGGING
  JAVA_SOURCE+=$(JAVA_PATH)/RaveFlavorTagFactory.java
endif

if HAS_KINEMATICS
  libJavaRave_la_LIBADD += ../src/RecoVertex/KinematicFit/libRaveVertexKinematics.la
  libJavaRave_la_CPPFLAGS += -DWITH_KINEMATICS
  SWIGOPTS+=-DWITH_KINEMATICS
  JAVA_SOURCE += \
    $(JAVA_PATH)/RaveVector4D.java \
    $(JAVA_PATH)/RavePerigeeParameters6D.java \
    $(JAVA_PATH)/RavePerigeeCovariance6D.java \
    $(JAVA_PATH)/RaveCovariance4D.java \
    $(JAVA_PATH)/RaveVector7D.java \
    $(JAVA_PATH)/RaveCovariance7D.java \
    $(JAVA_PATH)/RaveKParticle.java \
    $(JAVA_PATH)/RaveKTransientTrackParticle.java \
    $(JAVA_PATH)/RaveKVirtualParticle.java \
    $(JAVA_PATH)/RaveKParticleContainer.java \
    $(JAVA_PATH)/BasicKinematicConstraint.java \
    $(JAVA_PATH)/RaveKVertex.java \
    $(JAVA_PATH)/RaveKTree.java \
    $(JAVA_PATH)/RaveKTreeContainer.java \
    $(JAVA_PATH)/RaveKTreeFactory.java \
    $(JAVA_PATH)/RaveKConstraint.java \
    $(JAVA_PATH)/RaveKMultipleConstraint.java \
    $(JAVA_PATH)/RaveKConstraintBuilder.java \
    $(JAVA_PATH)/SWIGTYPE_p_boost__any.java \
    $(JAVA_PATH)/SWIGTYPE_p_boost__shared_ptrT*rave__KinematicConstraint_t.java \
    $(JAVA_PATH)/SWIGTYPE_p_boost__shared_ptrT*rave__MagneticField_t.java \
    $(JAVA_PATH)/SWIGTYPE_p_boost__weak_ptrT*rave__BasicKinematicTree_t.java \
    $(JAVA_PATH)/SWIGTYPE_p_rave__BasicKinematicTree.java \
    $(JAVA_PATH)/SWIGTYPE_p_rave__BasicKinematicVertex.java \
    $(JAVA_PATH)/SWIGTYPE_p_std__ostream.java \
    $(JAVA_PATH)/SWIGTYPE_p_std__pairT*bool_rave__KinematicParticle_t.java
endif

SWIG_OUTPUTS = rave.h $(JAVA_SOURCE)

${SWIG_OUTPUTS}: ${SWIG_SOURCES}

# rave.cc

# javashare_DATA = JavaRave.jar

EXTRA_DIST=${SWIG_OUTPUTS} rave.cc
# ${SWIG_OUTPUTS}
javashare_DATA = JavaRave.jar # ${SWIG_OUTPUTS}
# noinst_DATA = ${SWIG_OUTPUTS}

rave.cc: $(SWIG_SOURCES) $(top_srcdir)/configure.ac $(RAVEINCDIR)/Vertex.h  $(RAVEINCDIR)/Track.h $(RAVEINCDIR)/VertexFactory.h $(RAVEINCDIR)/FlavorTagFactory.h $(RAVEINCDIR)/KinematicTreeFactory.h
#$(srcdir)/at/hephy/Rave/rave.java: $(SWIG_SOURCES)
	mkdir -p at/hephy/Rave
	$(SWIG) -w401 -w319 -w473 $(JAVAOPTS) $(SWIGOPTS) $(SWIG_JAVA_OPT) -o rave.cc $<

CLEANFILES = rave.cc rave.h JavaRave.jar $(SWIG_OUTPUTS) $(JAVA_PATH)/*  *.class *.log

#MAINTAINERCLEANFILES = $(JAVA_PATH)/*.class

JAVACRUFT = JavaRave.jar

$(top_builddir)/rave_autoconf.i: /
	cd .. && make rave_autoconf.i
#../rave.i:
#	cd .. && make rave.i

JavaRave.jar: rave.cc
	rm -f $@
	$(JAVAC) -d . $(JAVA_SOURCE)
	$(JAR) cf $@ at
