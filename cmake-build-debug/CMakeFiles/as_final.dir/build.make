# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.FeYmIOPXsU

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.FeYmIOPXsU/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/as_final.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/as_final.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/as_final.dir/flags.make

CMakeFiles/as_final.dir/main.c.o: CMakeFiles/as_final.dir/flags.make
CMakeFiles/as_final.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.FeYmIOPXsU/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/as_final.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/as_final.dir/main.c.o   -c /tmp/tmp.FeYmIOPXsU/main.c

CMakeFiles/as_final.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/as_final.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.FeYmIOPXsU/main.c > CMakeFiles/as_final.dir/main.c.i

CMakeFiles/as_final.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/as_final.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.FeYmIOPXsU/main.c -o CMakeFiles/as_final.dir/main.c.s

CMakeFiles/as_final.dir/as/config.c.o: CMakeFiles/as_final.dir/flags.make
CMakeFiles/as_final.dir/as/config.c.o: ../as/config.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.FeYmIOPXsU/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/as_final.dir/as/config.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/as_final.dir/as/config.c.o   -c /tmp/tmp.FeYmIOPXsU/as/config.c

CMakeFiles/as_final.dir/as/config.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/as_final.dir/as/config.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.FeYmIOPXsU/as/config.c > CMakeFiles/as_final.dir/as/config.c.i

CMakeFiles/as_final.dir/as/config.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/as_final.dir/as/config.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.FeYmIOPXsU/as/config.c -o CMakeFiles/as_final.dir/as/config.c.s

CMakeFiles/as_final.dir/as/scan.c.o: CMakeFiles/as_final.dir/flags.make
CMakeFiles/as_final.dir/as/scan.c.o: ../as/scan.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.FeYmIOPXsU/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/as_final.dir/as/scan.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/as_final.dir/as/scan.c.o   -c /tmp/tmp.FeYmIOPXsU/as/scan.c

CMakeFiles/as_final.dir/as/scan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/as_final.dir/as/scan.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.FeYmIOPXsU/as/scan.c > CMakeFiles/as_final.dir/as/scan.c.i

CMakeFiles/as_final.dir/as/scan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/as_final.dir/as/scan.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.FeYmIOPXsU/as/scan.c -o CMakeFiles/as_final.dir/as/scan.c.s

# Object files for target as_final
as_final_OBJECTS = \
"CMakeFiles/as_final.dir/main.c.o" \
"CMakeFiles/as_final.dir/as/config.c.o" \
"CMakeFiles/as_final.dir/as/scan.c.o"

# External object files for target as_final
as_final_EXTERNAL_OBJECTS =

as_final: CMakeFiles/as_final.dir/main.c.o
as_final: CMakeFiles/as_final.dir/as/config.c.o
as_final: CMakeFiles/as_final.dir/as/scan.c.o
as_final: CMakeFiles/as_final.dir/build.make
as_final: CMakeFiles/as_final.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.FeYmIOPXsU/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable as_final"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/as_final.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/as_final.dir/build: as_final

.PHONY : CMakeFiles/as_final.dir/build

CMakeFiles/as_final.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/as_final.dir/cmake_clean.cmake
.PHONY : CMakeFiles/as_final.dir/clean

CMakeFiles/as_final.dir/depend:
	cd /tmp/tmp.FeYmIOPXsU/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.FeYmIOPXsU /tmp/tmp.FeYmIOPXsU /tmp/tmp.FeYmIOPXsU/cmake-build-debug /tmp/tmp.FeYmIOPXsU/cmake-build-debug /tmp/tmp.FeYmIOPXsU/cmake-build-debug/CMakeFiles/as_final.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/as_final.dir/depend

