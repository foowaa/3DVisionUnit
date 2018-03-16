cmake_minimum_required(VERSION 2.8)

project(SSCA C CXX)

#file(GLOB_RECURSE SRC_FILES "/home/nvidia/project/2/SSCA/*.cpp")
list(APPEND SRC_FILES /home/nvidia/project/2/SSCA/SSCA.cpp;/home/nvidia/project/2/SSCA/cvtools.cpp;/home/nvidia/project/2/SSCA/main1.cpp;/home/nvidia/project/2/SSCA/CC/CGCC.cpp;/home/nvidia/project/2/SSCA/CC/CenCC.cpp;/home/nvidia/project/2/SSCA/CC/GrdCC.cpp;/home/nvidia/project/2/SSCA/PPSG/SGPP.cpp;/home/nvidia/project/2/SSCA/PPWM/WMPP.cpp;/home/nvidia/project/2/SSCA/CAFilter/GuidedFilter.cpp;/home/nvidia/project/2/SSCA/CAFilter/GFCA.cpp;/home/nvidia/project/2/SSCA/CAFilter/BilateralFilter.cpp;/home/nvidia/project/2/SSCA/CAFilter/BFCA.cpp;/home/nvidia/project/2/SSCA/CAFilter/BoxCA.cpp;/home/nvidia/project/2/SSCA/CAST/ctmf1.c;/home/nvidia/project/2/SSCA/CAST/SegmentTree.cpp;/home/nvidia/project/2/SSCA/CAST/StereoDisparity.cpp;/home/nvidia/project/2/SSCA/CAST/STCA.cpp;/home/nvidia/project/2/SSCA/CAST/Toolkit.cpp;/home/nvidia/project/2/SSCA/CAST/StereoHelper.cpp)

message(STATUS "sources: ${SRC_FILES}")
include_directories(CAFilter CANLC CAST CC PPSG PPWM)
set(EXECUTABLE_OUTPUT_PATH /home/nvidia/project/2/SSCA/build/bin)
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

add_executable(SSCA ${SRC_FILES})

target_link_libraries(SSCA ${OpenCV_LIBS})

