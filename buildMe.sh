#!/bin/sh

make distclean
./bootstrap
./my-configure
make -j4 install
