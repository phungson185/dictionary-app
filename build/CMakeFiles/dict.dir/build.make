# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/dung/dictionary-app

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dung/dictionary-app/build

# Include any dependencies generated for this target.
include CMakeFiles/dict.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dict.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dict.dir/flags.make

CMakeFiles/dict.dir/src/dict.c.o: CMakeFiles/dict.dir/flags.make
CMakeFiles/dict.dir/src/dict.c.o: ../src/dict.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dung/dictionary-app/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/dict.dir/src/dict.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dict.dir/src/dict.c.o   -c /home/dung/dictionary-app/src/dict.c

CMakeFiles/dict.dir/src/dict.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dict.dir/src/dict.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dung/dictionary-app/src/dict.c > CMakeFiles/dict.dir/src/dict.c.i

CMakeFiles/dict.dir/src/dict.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dict.dir/src/dict.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dung/dictionary-app/src/dict.c -o CMakeFiles/dict.dir/src/dict.c.s

# Object files for target dict
dict_OBJECTS = \
"CMakeFiles/dict.dir/src/dict.c.o"

# External object files for target dict
dict_EXTERNAL_OBJECTS =

dict: CMakeFiles/dict.dir/src/dict.c.o
dict: CMakeFiles/dict.dir/build.make
dict: CMakeFiles/dict.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dung/dictionary-app/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable dict"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dict.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dict.dir/build: dict

.PHONY : CMakeFiles/dict.dir/build

CMakeFiles/dict.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dict.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dict.dir/clean

CMakeFiles/dict.dir/depend:
	cd /home/dung/dictionary-app/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dung/dictionary-app /home/dung/dictionary-app /home/dung/dictionary-app/build /home/dung/dictionary-app/build /home/dung/dictionary-app/build/CMakeFiles/dict.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dict.dir/depend

