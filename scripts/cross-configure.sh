#!/bin/sh

CONFIG_SHELL=/bin/sh
export CONFIG_SHELL
PREFIX=/usr/local/cross-tools
TARGET=i586-mingw32msvc
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
cache=cross-config.cache
cp scripts/libtool.hacked libtool

sh configure CXXFLAGS="" --prefix=$PREFIX --enable-flavortagging --disable-java --with-clhep=$PREFIX --with-clhep-libpath=$PREFIX/lib --with-clhep-incpath=$PREFIX/include --with-boost=$PREFIX --with-clhep_vector-libpath=$PREFIX/lib --with-clhep_matrix-libpath=$PREFIX/lib --cache-file="$cache" \
	--target=$TARGET --host=$TARGET --build=i386-linux \
	$*
status=$?
rm -f "$cache"
exit $status

