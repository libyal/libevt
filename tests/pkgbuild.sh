#!/bin/sh
# Script to build a MacOS pkg
#
# Version: 20241015

set -e

make install DESTDIR=${PWD}/osx-pkg
mkdir -p ${PWD}/osx-pkg/usr/share/doc/libevt
cp AUTHORS COPYING COPYING.LESSER NEWS README ${PWD}/osx-pkg/usr/share/doc/libevt

VERSION=`sed '5!d; s/^  \[//;s/\],$//' configure.ac`
pkgbuild --root osx-pkg --identifier com.github.libyal.libevt --version ${VERSION} --ownership recommended ../libevt-${VERSION}.pkg

