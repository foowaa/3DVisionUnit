cmake_minimum_required(VERSION 3.2)
project(test1)

set(CMAKE_CXX_STANDARD 11)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set( CMAKE_CXX_FLAGS "-std=c++11 -O3")
include(/usr/bin/cmake-3.10.3/Modules/FindCUDA.cmake)

find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(./inc)

set(SOURCE_FILES test.cu headers.h)
cuda_add_library(test1 ${SOURCE_FILES})

#message(${CUDA_LIBRARIES})
link_directories(/usr/local/cuda/lib64/)
set(CUDA_DIR /usr/local/cuda/lib64)

target_link_libraries(test1 ${OpenCV_LIBS})
target_link_libraries(test1 ${CUDA_LIBRARIES})
target_link_libraries(test1 libnppc.so libnppi.so libcublas.so libcusolver.so)
