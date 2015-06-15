#!/bin/sh

PREFIX=/usr/local/cross-tools
TARGET=i586-mingw32msvc
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
exec make $*
