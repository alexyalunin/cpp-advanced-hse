# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse

# Include any dependencies generated for this target.
include tasks/stdflib/CMakeFiles/test_stdflib.dir/depend.make
# Include the progress variables for this target.
include tasks/stdflib/CMakeFiles/test_stdflib.dir/progress.make

# Include the compile flags for this target's objects.
include tasks/stdflib/CMakeFiles/test_stdflib.dir/flags.make

tasks/stdflib/CMakeFiles/test_stdflib.dir/test.cpp.obj: tasks/stdflib/CMakeFiles/test_stdflib.dir/flags.make
tasks/stdflib/CMakeFiles/test_stdflib.dir/test.cpp.obj: tasks/stdflib/CMakeFiles/test_stdflib.dir/includes_CXX.rsp
tasks/stdflib/CMakeFiles/test_stdflib.dir/test.cpp.obj: tasks/stdflib/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=\\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tasks/stdflib/CMakeFiles/test_stdflib.dir/test.cpp.obj"
	cd /d \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_stdflib.dir\test.cpp.obj -c \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib\test.cpp

tasks/stdflib/CMakeFiles/test_stdflib.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_stdflib.dir/test.cpp.i"
	cd /d \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib\test.cpp > CMakeFiles\test_stdflib.dir\test.cpp.i

tasks/stdflib/CMakeFiles/test_stdflib.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_stdflib.dir/test.cpp.s"
	cd /d \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib\test.cpp -o CMakeFiles\test_stdflib.dir\test.cpp.s

# Object files for target test_stdflib
test_stdflib_OBJECTS = \
"CMakeFiles/test_stdflib.dir/test.cpp.obj"

# External object files for target test_stdflib
test_stdflib_EXTERNAL_OBJECTS =

test_stdflib.exe: tasks/stdflib/CMakeFiles/test_stdflib.dir/test.cpp.obj
test_stdflib.exe: tasks/stdflib/CMakeFiles/test_stdflib.dir/build.make
test_stdflib.exe: libcontrib_catch_main.a
test_stdflib.exe: tasks/stdflib/CMakeFiles/test_stdflib.dir/linklibs.rsp
test_stdflib.exe: tasks/stdflib/CMakeFiles/test_stdflib.dir/objects1.rsp
test_stdflib.exe: tasks/stdflib/CMakeFiles/test_stdflib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=\\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ..\..\test_stdflib.exe"
	cd /d \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\test_stdflib.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tasks/stdflib/CMakeFiles/test_stdflib.dir/build: test_stdflib.exe
.PHONY : tasks/stdflib/CMakeFiles/test_stdflib.dir/build

tasks/stdflib/CMakeFiles/test_stdflib.dir/clean:
	cd /d \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib && $(CMAKE_COMMAND) -P CMakeFiles\test_stdflib.dir\cmake_clean.cmake
.PHONY : tasks/stdflib/CMakeFiles/test_stdflib.dir/clean

tasks/stdflib/CMakeFiles/test_stdflib.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib \\wsl$$\Ubuntu-20.04\home\dnsost\cpp-advanced-hse\tasks\stdflib\CMakeFiles\test_stdflib.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : tasks/stdflib/CMakeFiles/test_stdflib.dir/depend
