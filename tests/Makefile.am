AM_CPPFLAGS = $(PLATFORMCPPFLAGS) -I$(top_srcdir)/src/ -I$(top_srcdir)/src/RaveBase/RaveInterface $(BOOST_CPPFLAGS) -I$(LHEPINCPATH) -I$(CLHEPINCPATH) -I$(top_srcdir)/src/ROOT/smatrix/ -I$(top_srcdir)/src/ROOT/mathcore/ -I$(top_srcdir)/src/ROOT/ -I$(top_srcdir)/src/ROOT/genvector/ -DRAVE

bin_PROGRAMS = rave_version

$(top_builddir)/src/RaveBase/libRaveBase.la:
	cd $(top_builddir)/src/RaveBase/ && make libRaveBase.la

$(top_builddir)/src/RecoBTag/libRaveFlavorTag.la:
	cd $(top_builddir)/src/RecoBtag && make libRaveFlavorTag.la

$(top_builddir)/src/RecoVertex/KinematicFit/libRaveVertexKinematics.la:
	cd $(top_builddir)/src/RecoVertex/KinematicFit && make libRaveVertexKinematics.la

PLATFORMLDADD = $(top_builddir)/src/RaveBase/libRaveBase.la $(top_builddir)/src/libRaveCore.la $(top_builddir)/src/RecoVertex/libRaveVertex.la $(CLHEPLIB)

if HAS_KINEMATICS
  PLATFORMLDADD += $(top_builddir)/src/RecoVertex/KinematicFit/libRaveVertexKinematics.la
  bin_PROGRAMS  += kinematics01 kinematics02 kinematics03 memorytests propagatortests
endif

if HAS_FLAVORTAGGING
  PLATFORMLDADD += $(top_builddir)/src/RecoBTag/libRaveFlavorTag.la
endif

rave_version_SOURCES = rave_version.cc
rave_version_LDFLAGS = $(PLATFORMLDFLAGS)
rave_version_LDADD   = $(PLATFORMLDADD)

kinematics01_SOURCES = kinematics01.cc
kinematics01_LDFLAGS = $(PLATFORMLDFLAGS)
kinematics01_LDADD   = $(PLATFORMLDADD)

kinematics02_SOURCES = kinematics02.cc
kinematics02_LDFLAGS = $(PLATFORMLDFLAGS)
kinematics02_LDADD   = $(PLATFORMLDADD)

kinematics03_SOURCES = kinematics03.cc
kinematics03_LDFLAGS = $(PLATFORMLDFLAGS)
kinematics03_LDADD   = $(PLATFORMLDADD)


memorytests_SOURCES = memorytests.cc
memorytests_LDFLAGS = $(PLATFORMLDFLAGS)
memorytests_LDADD   = $(PLATFORMLDADD)

propagatortests_SOURCES = propagatortests.cc
propagatortests_LDFLAGS = $(PLATFORMLDFLAGS)
propagatortests_LDADD   = $(PLATFORMLDADD)

windows: rave_windows.o
	$(CXX) -mno-cygwin rave_windows.o -L${prefix}/bin -mwindows -lRaveCore -lRaveVertex -lRaveBase -lCLHEP -Wl,--enable-auto-import --whole-archive -Wl,--export-all-symbols -o rave_windows
