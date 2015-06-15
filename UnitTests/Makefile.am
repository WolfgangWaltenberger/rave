AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/RaveBase/RaveInterface -I$(top_srcdir)/UnitTests/ $(BOOST_CPPFLAGS) -I$(CLHEPINCPATH) $(PLATFORMCPPFLAGS) -I$(CLHEPINCPATH) -I$(top_srcdir)/src/ROOT/smatrix/ -I$(top_srcdir)/src/ROOT/mathcore/ -I$(top_srcdir)/src/ROOT/ -I$(top_srcdir)/src/ROOT/genvector/ -DRAVE

$(top_builddir)/src/RaveBase/libRaveBase.la:                                             
	cd $(top_builddir)/src/RaveBase/ && make libRaveBase.la        

$(top_builddir)/src/RecoBTag/libRaveFlavorTag.la:
	cd $(top_builddir)/src/RecoBTag && make libRaveFlavorTag.la

bin_PROGRAMS = raveunittest

PLATFORMLDADD = -lboost_unit_test_framework  $(top_builddir)/src/RaveBase/libRaveBase.la $(top_builddir)/src/libRaveCore.la $(top_builddir)/src/RecoVertex/libRaveVertex.la $(CLHEPLIB)

raveunittest_SOURCES = LowMomentumScale.cxx \
RaveTest.cxx \
unittest.cxx \
PropertyHandler.cxx \
TinyBField.cxx \
ZeroBField.cxx

noinst_HEADERS = PropertyHandler.h RaveTest.h

raveunittest_LDFLAGS = $(PLATFORMLDFLAGS)
raveunittest_LDADD = $(PLATFORMLDADD)

