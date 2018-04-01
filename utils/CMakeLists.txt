cmake_minimum_required(VERSION 2.8)

project(depth2cloud C CXX)

#message(STATUS "sources: ${SRC_FILES}")
#include_directories(CAFilter CANLC CAST CC PPSG PPWM)
set(EXECUTABLE_OUTPUT_PATH PROJECT_SOURCE_DIR/lib)
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

add_library(depth2cloud STATIC  depth2cloud.cpp)

target_link_libraries(depth2cloud ${OpenCV_LIBS})

