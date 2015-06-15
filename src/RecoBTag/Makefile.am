# JAVAC_FLAGS is needed, to enable -fno-strict-aliasing when interfacing to java.
AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/ROOT/genvector/  -I$(top_srcdir)/src/ROOT/mathcore/ -I$(top_srcdir)/src/ROOT/smatrix/ -I$(top_srcdir)/src/ROOT/ -I$(top_srcdir)/src/RaveBase/RaveInterface -I$(CLHEPINCPATH) $(BOOST_CPPFLAGS) -DRAVE $(FLAVORTAGGING_CPPFLAGS) $(JAVAC_FLAGS) $(PLATFORMCPPFLAGS)

lib_LTLIBRARIES=libRaveFlavorTag.la

libRaveFlavorTag_la_LDFLAGS = $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) $(PLATFORMLDFLAGS)
libRaveFlavorTag_la_LIBADD = ../RecoVertex/libRaveVertex.la ../libRaveCore.la 

nobase_implinclude_HEADERS = \
../DataFormats/BTauReco/interface/TaggingVariable.h \
../DataFormats/BTauReco/interface/TaggingVariableFwd.h \
../DataFormats/BTauReco/interface/RefMacros.h \
../DataFormats/BTauReco/interface/BaseTagInfo.h \
../DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h \
SecondaryVertex/interface/SecondaryVertex.h \
SecondaryVertex/interface/VertexSorting.h 

noinst_HEADERS = \
../DataFormats/BTauReco/interface/JetTag.h \
../DataFormats/BTauReco/interface/JetTagFwd.h \
../DataFormats/BTauReco/interface/JetTracksAssociation.h \
../RecoBTau/JetTagComputer/interface/JetTagComputer.h \
SecondaryVertex/interface/ParticleMasses.h \
SecondaryVertex/interface/SVBuilder.h \
SecondaryVertex/interface/SVFilter.h \
SecondaryVertex/interface/SecondaryVertex.h \
SecondaryVertex/interface/V0Filter.h \
SecondaryVertex/interface/VertexFilter.h \
SecondaryVertex/interface/SimpleVertexBasedTagger.h \
SecondaryVertex/interface/SimpleSecondaryVertexComputer.h \
SecondaryVertex/interface/TrackKinematics.h \
SecondaryVertex/interface/TrackSorting.h \
SecondaryVertex/interface/TrackSelector.h

libRaveFlavorTag_la_SOURCES = SecondaryVertex/src/ParticleMasses.cc \
SecondaryVertex/src/SecondaryVertex.cc \
SecondaryVertex/src/V0Filter.cc \
SecondaryVertex/src/VertexSorting.cc \
SecondaryVertex/src/VertexFilter.cc \
SecondaryVertex/src/TrackKinematics.cc \
SecondaryVertex/src/TrackSorting.cc \
SecondaryVertex/src/TrackSelector.cc \
SecondaryVertex/src/SimpleVertexBasedTagger.cc \
SecondaryVertex/src/SVBuilder.cc \
SecondaryVertex/src/SVFilter.cc \
../DataFormats/BTauReco/src/SecondaryVertexTagInfo.cc \
../DataFormats/BTauReco/src/JetTag.cc \
../DataFormats/BTauReco/src/TaggingVariable.cc 

implincludedir = $(includedir)/rave/impl/RecoBTag

if IS_WIN32
FLAVOROBJS=$(libRaveFlavorTag_la_OBJECTS:.lo=.o)

dll: libRaveFlavorTag.la
	$(CXX) -shared -L${prefix}/bin $(libRaveFlavorTag_la_LDFLAGS) $(FLAVOROBJS) -o${prefix}/bin/libRaveFlavorTag.dll $(CLHEPLDFLAGS) -l$(CLHEPLIBNAME) -lRaveCore -lRaveVertex -Wl,--rpath -Wl,${prefix}/bin/
endif
