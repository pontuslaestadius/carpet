# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /home/pontus/git/carpet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pontus/git/carpet

# Include any dependencies generated for this target.
include CMakeFiles/carpet.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/carpet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/carpet.dir/flags.make

CMakeFiles/carpet.dir/main.cpp.o: CMakeFiles/carpet.dir/flags.make
CMakeFiles/carpet.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pontus/git/carpet/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/carpet.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/carpet.dir/main.cpp.o -c /home/pontus/git/carpet/main.cpp

CMakeFiles/carpet.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/carpet.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pontus/git/carpet/main.cpp > CMakeFiles/carpet.dir/main.cpp.i

CMakeFiles/carpet.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/carpet.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pontus/git/carpet/main.cpp -o CMakeFiles/carpet.dir/main.cpp.s

CMakeFiles/carpet.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/carpet.dir/main.cpp.o.requires

CMakeFiles/carpet.dir/main.cpp.o.provides: CMakeFiles/carpet.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/carpet.dir/build.make CMakeFiles/carpet.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/carpet.dir/main.cpp.o.provides

CMakeFiles/carpet.dir/main.cpp.o.provides.build: CMakeFiles/carpet.dir/main.cpp.o


# Object files for target carpet
carpet_OBJECTS = \
"CMakeFiles/carpet.dir/main.cpp.o"

# External object files for target carpet
carpet_EXTERNAL_OBJECTS =

carpet: CMakeFiles/carpet.dir/main.cpp.o
carpet: CMakeFiles/carpet.dir/build.make
carpet: CMakeFiles/carpet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pontus/git/carpet/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable carpet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/carpet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/carpet.dir/build: carpet

.PHONY : CMakeFiles/carpet.dir/build

CMakeFiles/carpet.dir/requires: CMakeFiles/carpet.dir/main.cpp.o.requires

.PHONY : CMakeFiles/carpet.dir/requires

CMakeFiles/carpet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/carpet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/carpet.dir/clean

CMakeFiles/carpet.dir/depend:
	cd /home/pontus/git/carpet && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pontus/git/carpet /home/pontus/git/carpet /home/pontus/git/carpet /home/pontus/git/carpet /home/pontus/git/carpet/CMakeFiles/carpet.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/carpet.dir/depend

