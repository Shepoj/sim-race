#!/bin/bash

cmake --version

if [ -z $1 ]; then
	echo Missing Config param
	exit 1
fi

src=$(pwd)
prj=$(basename $src)
cfg=$1
opt=$2
installPath=$src/../install #run_$cfg$opt

mkdir -p ../build
mkdir -p ../build/$prj
mkdir -p ../build/$prj/$cfg$opt
mkdir -p $installPath

cd ../build/$prj/$cfg$opt

cmake -DCMAKE_BUILD_TYPE=$cfg -DSFML_BUILD_AUDIO=FALSE -DCMAKE_INSTALL_PREFIX=$installPath $src

cmake --build . --config $cfg -j 15
cmake --build . --target install --config $cfg
