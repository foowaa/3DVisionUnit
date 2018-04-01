cmake_minimum_required(VERSION 2.8)

project(SSCA C CXX)

#file(GLOB_RECURSE SRC_FILES "../*.cpp")
list(APPEND SRC_FILES ../SSCA.cpp;../cvtools.cpp;../CC/CGCC.cpp;../CC/CenCC.cpp;../CC/GrdCC.cpp;../PPSG/SGPP.cpp;../PPWM/WMPP.cpp;../CAFilter/GuidedFilter.cpp;../CAFilter/GFCA.cpp;../CAFilter/BilateralFilter.cpp;../CAFilter/BFCA.cpp;../CAFilter/BoxCA.cpp;../CAST/ctmf1.c;../CAST/SegmentTree.cpp;../CAST/StereoDisparity.cpp;../CAST/STCA.cpp;../CAST/Toolkit.cpp;../CAST/StereoHelper.cpp;../main1.cpp)

message(STATUS "sources: ${SRC_FILES}")
include_directories(CAFilter CANLC CAST CC PPSG PPWM)
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

add_executable(SSCA ${SRC_FILES})
#add_library(SSCA ${SRC_FILES})
#target_link_libraries(SSCA ${OpenCV_LIBS} ../lib/libSSCA.so )
target_link_libraries(SSCA ${OpenCV_LIBS})
