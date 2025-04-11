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
installPath=./run_$cfg$opt

cd ../build/$prj/$cfg$opt

cmake --build . --config $cfg -j 15
cmake --build . --target install --config $cfg
