#!/bin/bash


func_build(){
    if [ ! -d "./build" ]; then
        mkdir ./build
    else
    	cd ./build
    	rm -rf *
    fi
}

source /etc/profile

while getopts "abcde" opt
do
	case $opt in
	a) 
           echo "******************************ALL********************************"
           cp ./cmake/library/SSCA/CMakeLists.txt ./SSCA
	   cp ./cmake/library/grid/CMakeLists.txt ./grid
	   cp ./cmake/library/utils/CMakeLists.txt ./utils
           func_build
	   echo "start cmake..."
	   cmake ..
	   echo "start make..."
           make           
        ;;
	b) 
           echo "******************************SSCA********************************"
           cp ./cmake/execution/SSCA/CMakeLists.txt ./SSCA
           cd SSCA
           func_build
	   echo "start cmake..."
	   cmake ..
	   echo "start make..."
           make  
        ;;
	c) 
           echo "******************************grid********************************"
           cp ./cmake/execution/grid/CMakeLists.txt ./grid
           cd grid
           func_build
	   echo "start cmake..."
	   cmake ..
	   echo "start make..."
           make  
        ;;
	d) 
           echo "******************************utils********************************"
           cp ./cmake/execution/utils/CMakeLists.txt ./utils
           cd ./utils
           func_build
	   echo "start cmake..."
	   cmake ..
	   echo "start make..."
           make  
        ;;
        e) 
           echo "******************************preproc********************************"
           cp ./cmake/execution/preproc/CMakeLists.txt ./preproc
           cd ./preproc
           func_build
	   echo "start cmake..."
	   cmake ..
	   echo "start make..."
           make  
        ;;
	*) echo "Unknown option: $opt";;
	esac
done


