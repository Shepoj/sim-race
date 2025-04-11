#!/bin/bash

cmake --version

if [ -z $1 ]; then
	echo Missing Config param
	exit 1
fi

src=$(pwd)
prj=$(basename $src)
cfg=$1
installPath=$src/../runApp

echo $installPath

mkdir -p ../build
mkdir -p ../build/$prj
mkdir -p ../build/$prj/$cfg$opt
mkdir -p $installPath

cd ../build/$prj/$cfg

cmake -DCMAKE_BUILD_TYPE=$cfg -DCMAKE_PREFIX_PATH="../SFML/install/lib/cmake;../libphysics/lib/cmake" -DCMAKE_INSTALL_PREFIX=$installPath $src

