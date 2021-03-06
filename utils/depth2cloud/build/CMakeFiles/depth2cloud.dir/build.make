# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nvidia/SuperVehicle/utils/depth2cloud

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nvidia/SuperVehicle/utils/depth2cloud/build

# Include any dependencies generated for this target.
include CMakeFiles/depth2cloud.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/depth2cloud.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/depth2cloud.dir/flags.make

CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o: CMakeFiles/depth2cloud.dir/flags.make
CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o: ../depth2cloud.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nvidia/SuperVehicle/utils/depth2cloud/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o -c /home/nvidia/SuperVehicle/utils/depth2cloud/depth2cloud.cpp

CMakeFiles/depth2cloud.dir/depth2cloud.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/depth2cloud.dir/depth2cloud.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nvidia/SuperVehicle/utils/depth2cloud/depth2cloud.cpp > CMakeFiles/depth2cloud.dir/depth2cloud.cpp.i

CMakeFiles/depth2cloud.dir/depth2cloud.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/depth2cloud.dir/depth2cloud.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nvidia/SuperVehicle/utils/depth2cloud/depth2cloud.cpp -o CMakeFiles/depth2cloud.dir/depth2cloud.cpp.s

CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o.requires:
.PHONY : CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o.requires

CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o.provides: CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o.requires
	$(MAKE) -f CMakeFiles/depth2cloud.dir/build.make CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o.provides.build
.PHONY : CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o.provides

CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o.provides.build: CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o

# Object files for target depth2cloud
depth2cloud_OBJECTS = \
"CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o"

# External object files for target depth2cloud
depth2cloud_EXTERNAL_OBJECTS =

libdepth2cloud.so: CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o
libdepth2cloud.so: CMakeFiles/depth2cloud.dir/build.make
libdepth2cloud.so: /usr/lib/libopencv_vstab.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_tegra.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_imuvstab.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_facedetect.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_esm_panorama.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_detection_based_tracker.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_videostab.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_video.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_ts.a
libdepth2cloud.so: /usr/lib/libopencv_superres.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_stitching.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_photo.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_objdetect.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_ml.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_legacy.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_imgproc.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_highgui.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_gpu.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_flann.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_features2d.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_core.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_contrib.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_calib3d.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_tegra.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_stitching.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_gpu.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_photo.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_legacy.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_video.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_objdetect.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_ml.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_calib3d.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_features2d.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_highgui.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_imgproc.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_flann.so.2.4.13
libdepth2cloud.so: /usr/lib/libopencv_core.so.2.4.13
libdepth2cloud.so: CMakeFiles/depth2cloud.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library libdepth2cloud.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/depth2cloud.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/depth2cloud.dir/build: libdepth2cloud.so
.PHONY : CMakeFiles/depth2cloud.dir/build

CMakeFiles/depth2cloud.dir/requires: CMakeFiles/depth2cloud.dir/depth2cloud.cpp.o.requires
.PHONY : CMakeFiles/depth2cloud.dir/requires

CMakeFiles/depth2cloud.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/depth2cloud.dir/cmake_clean.cmake
.PHONY : CMakeFiles/depth2cloud.dir/clean

CMakeFiles/depth2cloud.dir/depend:
	cd /home/nvidia/SuperVehicle/utils/depth2cloud/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nvidia/SuperVehicle/utils/depth2cloud /home/nvidia/SuperVehicle/utils/depth2cloud /home/nvidia/SuperVehicle/utils/depth2cloud/build /home/nvidia/SuperVehicle/utils/depth2cloud/build /home/nvidia/SuperVehicle/utils/depth2cloud/build/CMakeFiles/depth2cloud.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/depth2cloud.dir/depend

