#!/bin/bash

while getopts :ab:cd opt
do
	case "$opt" in
	-n) echo "Found the -n option,with value $OPTARG";;
#	b) echo "Found the -b option,with value $OPTARG";;
#	c) echo "Found the -c option";;
#	d) echo "Found the -d option";;
	*) echo "Unknown option: $opt";;
	esac
done
a = "all"
b = "SSCA"
c = "grid"
d = "utils"
e = "preproc"

function func_build {
    if [ ! -d "./build" ]; then
        mkdir ./build
    else
    	cd ./build
    	rm -rf *
    fi
}

if [[ $OPTARG = $a ]]; then
	func_build
	cp ./cmake/library/SSCA/CMakeLists.txt ./SSCA
	cp ./cmake/library/grid/CMakeLists.txt ./grid
	cp ./cmake/library/utils/CMakeLists.txt ./utils
    echo "strat cmake..."
    cmake ..
    echo "strat make..."
    make
elif [[ $OPTARG = $b ]]; then
	cp ./cmake/execution/SSCA/CMakeLists.txt ./SSCA
	cd ./SSCA
    func_build
    echo "strat cmake..."
    cmake ..
    echo "strat make..."
    make
elif [[ $OPTARG = $c ]]; then
	cp ./cmake/execution/grid/CMakeLists.txt ./grid
	cd ./grid
    func_build
    echo "strat cmake..."
    cmake ..
    echo "strat make..."
    make
elif [[ $OPTARG = $b ]]; then
	cp ./cmake/execution/utils/CMakeLists.txt ./utils
	cd ./utils
    func_build
    echo "strat cmake..."
    cmake ..
    echo "strat make..."
    make
elif [[ $OPTARG = $b ]]; then
	cp ./cmake/execution/preproc/CMakeLists.txt ./grid
	cd ./preproc
    func_build
    echo "strat cmake..."
    cmake ..
    echo "strat make..."
    make
else
	echo "ERROR in parameters!"
fi

