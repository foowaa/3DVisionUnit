cmake_minimum_required(VERSION 3.2)

project(SSCA)
set(CMAKE_CXX_STANDARD 11)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(OpenMP)
if (OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGES} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGES} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#file(GLOB_RECURSE SRC_FILES "../*.cpp")
list(APPEND SRC_FILES ../SSCA.cpp;../cvtools.cpp;../CC/CGCC.cpp;../CC/CenCC.cpp;../CC/GrdCC.cpp;../PPSG/SGPP.cpp;../PPWM/WMPP.cpp;../CAFilter/GuidedFilter.cpp;../CAFilter/GFCA.cpp;../CAFilter/BilateralFilter.cpp;../CAFilter/BFCA.cpp;../CAFilter/BoxCA.cpp;../CAST/ctmf1.c;../CAST/SegmentTree.cpp;../CAST/StereoDisparity.cpp;../CAST/STCA.cpp;../CAST/Toolkit.cpp;../CAST/StereoHelper.cpp;../main1.cpp)

include(/usr/bin/cmake-3.10.3/Modules/FindCUDA.cmake)
find_package(CUDA REQUIRED)
include_directories(./inc)  

message(STATUS "sources: ${SRC_FILES}")
include_directories(CAFilter CANLC CAST CC PPSG PPWM inc)
set(EXECUTABLE_OUTPUT_PATH ./bin)
# Find OpenCV, you may need to set variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
set(OpenCV_DIR '/usr/share/OpenCV/')

find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})

cuda_add_executable(SSCA ${SRC_FILES})

#add_executable(SSCA ${SRC_FILES})

link_directories(/usr/local/cuda/lib64/)
message(${CUDA_LIBRARIES})

#add_library(SSCA ${SRC_FILES})
#target_link_libraries(SSCA ${OpenCV_LIBS} ../lib/libSSCA.so )
target_link_libraries(SSCA ${OpenCV_LIBS} )
target_link_libraries(SSCA ${CUDA_LIBRARIES})
target_link_libraries(SSCA /home/nvidia/SuperVehicle1.2/SSCA/CAFilter/libGuidedFilterOptimize.a)
