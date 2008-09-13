#!/bin/sh

cd `dirname $0`

export SCRIPTDIR=$PWD
export BUILDDIR=$PWD/build

mkdir -p $BUILDDIR
cd $BUILDDIR
qmake -r $SCRIPTDIR/sforms.pro
make
