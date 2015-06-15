ACLOCAL_AMFLAGS = -I m4 -I m4/cedar

SUBDIRS = src src/RecoVertex

if HAS_UNITTESTS
  SUBDIRS += UnitTests
endif

if HAS_FLAVORTAGGING
  SUBDIRS+=src/RecoBTag
endif

SUBDIRS+=src/RaveBase tests

if JAVA_ENABLED
  SUBDIRS+= java
endif

# rules for pkg-config
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = rave.pc rave-impl.pc

swighdrdir = $(includedir)/rave/swig
swighdr_DATA = rave_autoconf.i rave_vcbuild.i rave.i

# install cmake configuration file
cmakeconfigdir = $(pkgdatadir)
cmakeconfig_DATA = RaveConfig.cmake

EXTRA_DIST = rave_autoconf.i rave_vcbuild.i rave.i vcconfig.h Rave.sln Rave.vcproj

$(pkgconfig_DATA): config.status

rpmbuild:
	./configure
	make clean
	rm -rf /tmp/rave-${VERSION}
	cp -r . /tmp/rave-${VERSION}
	cd /tmp && tar czvf /usr/src/redhat/SOURCES/rave-${VERSION}.tar.gz rave-${VERSION}
	rpmbuild -bb rave.spec

# mkdir -p /usr/src/redhat/BUILD/rave-${VERSION}/

if IS_WIN32
dlls:
	cd src && $(MAKE) dlls
endif

# dist dist-zip
hepforge_distupload:
	test -e rave-${VERSION}.tar.gz && scp rave-${VERSION}.tar.gz login.hepforge.org:rave/downloads
	test -e rave-${VERSION}.zip && scp rave-${VERSION}.zip login.hepforge.org:rave/downloads
