# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/runner/work/_temp/773599641/cmake-3.20.1-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/runner/work/_temp/773599641/cmake-3.20.1-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/runner/work/marbellous/marbellous/backend

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/runner/work/marbellous/marbellous/backend

# Include any dependencies generated for this target.
include CMakeFiles/Marbellous.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Marbellous.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Marbellous.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Marbellous.dir/flags.make

CMakeFiles/Marbellous.dir/src/Color.cpp.o: CMakeFiles/Marbellous.dir/flags.make
CMakeFiles/Marbellous.dir/src/Color.cpp.o: CMakeFiles/Marbellous.dir/includes_CXX.rsp
CMakeFiles/Marbellous.dir/src/Color.cpp.o: src/Color.cpp
CMakeFiles/Marbellous.dir/src/Color.cpp.o: CMakeFiles/Marbellous.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/marbellous/marbellous/backend/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Marbellous.dir/src/Color.cpp.o"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Marbellous.dir/src/Color.cpp.o -MF CMakeFiles/Marbellous.dir/src/Color.cpp.o.d -o CMakeFiles/Marbellous.dir/src/Color.cpp.o -c /home/runner/work/marbellous/marbellous/backend/src/Color.cpp

CMakeFiles/Marbellous.dir/src/Color.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Marbellous.dir/src/Color.cpp.i"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/marbellous/marbellous/backend/src/Color.cpp > CMakeFiles/Marbellous.dir/src/Color.cpp.i

CMakeFiles/Marbellous.dir/src/Color.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Marbellous.dir/src/Color.cpp.s"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/marbellous/marbellous/backend/src/Color.cpp -o CMakeFiles/Marbellous.dir/src/Color.cpp.s

CMakeFiles/Marbellous.dir/src/Options.cpp.o: CMakeFiles/Marbellous.dir/flags.make
CMakeFiles/Marbellous.dir/src/Options.cpp.o: CMakeFiles/Marbellous.dir/includes_CXX.rsp
CMakeFiles/Marbellous.dir/src/Options.cpp.o: src/Options.cpp
CMakeFiles/Marbellous.dir/src/Options.cpp.o: CMakeFiles/Marbellous.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/marbellous/marbellous/backend/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Marbellous.dir/src/Options.cpp.o"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Marbellous.dir/src/Options.cpp.o -MF CMakeFiles/Marbellous.dir/src/Options.cpp.o.d -o CMakeFiles/Marbellous.dir/src/Options.cpp.o -c /home/runner/work/marbellous/marbellous/backend/src/Options.cpp

CMakeFiles/Marbellous.dir/src/Options.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Marbellous.dir/src/Options.cpp.i"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/marbellous/marbellous/backend/src/Options.cpp > CMakeFiles/Marbellous.dir/src/Options.cpp.i

CMakeFiles/Marbellous.dir/src/Options.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Marbellous.dir/src/Options.cpp.s"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/marbellous/marbellous/backend/src/Options.cpp -o CMakeFiles/Marbellous.dir/src/Options.cpp.s

CMakeFiles/Marbellous.dir/src/Palette.cpp.o: CMakeFiles/Marbellous.dir/flags.make
CMakeFiles/Marbellous.dir/src/Palette.cpp.o: CMakeFiles/Marbellous.dir/includes_CXX.rsp
CMakeFiles/Marbellous.dir/src/Palette.cpp.o: src/Palette.cpp
CMakeFiles/Marbellous.dir/src/Palette.cpp.o: CMakeFiles/Marbellous.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/marbellous/marbellous/backend/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Marbellous.dir/src/Palette.cpp.o"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Marbellous.dir/src/Palette.cpp.o -MF CMakeFiles/Marbellous.dir/src/Palette.cpp.o.d -o CMakeFiles/Marbellous.dir/src/Palette.cpp.o -c /home/runner/work/marbellous/marbellous/backend/src/Palette.cpp

CMakeFiles/Marbellous.dir/src/Palette.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Marbellous.dir/src/Palette.cpp.i"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/marbellous/marbellous/backend/src/Palette.cpp > CMakeFiles/Marbellous.dir/src/Palette.cpp.i

CMakeFiles/Marbellous.dir/src/Palette.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Marbellous.dir/src/Palette.cpp.s"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/marbellous/marbellous/backend/src/Palette.cpp -o CMakeFiles/Marbellous.dir/src/Palette.cpp.s

CMakeFiles/Marbellous.dir/src/Polygon.cpp.o: CMakeFiles/Marbellous.dir/flags.make
CMakeFiles/Marbellous.dir/src/Polygon.cpp.o: CMakeFiles/Marbellous.dir/includes_CXX.rsp
CMakeFiles/Marbellous.dir/src/Polygon.cpp.o: src/Polygon.cpp
CMakeFiles/Marbellous.dir/src/Polygon.cpp.o: CMakeFiles/Marbellous.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/marbellous/marbellous/backend/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Marbellous.dir/src/Polygon.cpp.o"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Marbellous.dir/src/Polygon.cpp.o -MF CMakeFiles/Marbellous.dir/src/Polygon.cpp.o.d -o CMakeFiles/Marbellous.dir/src/Polygon.cpp.o -c /home/runner/work/marbellous/marbellous/backend/src/Polygon.cpp

CMakeFiles/Marbellous.dir/src/Polygon.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Marbellous.dir/src/Polygon.cpp.i"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/marbellous/marbellous/backend/src/Polygon.cpp > CMakeFiles/Marbellous.dir/src/Polygon.cpp.i

CMakeFiles/Marbellous.dir/src/Polygon.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Marbellous.dir/src/Polygon.cpp.s"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/marbellous/marbellous/backend/src/Polygon.cpp -o CMakeFiles/Marbellous.dir/src/Polygon.cpp.s

CMakeFiles/Marbellous.dir/src/WGLContext.cpp.o: CMakeFiles/Marbellous.dir/flags.make
CMakeFiles/Marbellous.dir/src/WGLContext.cpp.o: CMakeFiles/Marbellous.dir/includes_CXX.rsp
CMakeFiles/Marbellous.dir/src/WGLContext.cpp.o: src/WGLContext.cpp
CMakeFiles/Marbellous.dir/src/WGLContext.cpp.o: CMakeFiles/Marbellous.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/marbellous/marbellous/backend/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Marbellous.dir/src/WGLContext.cpp.o"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Marbellous.dir/src/WGLContext.cpp.o -MF CMakeFiles/Marbellous.dir/src/WGLContext.cpp.o.d -o CMakeFiles/Marbellous.dir/src/WGLContext.cpp.o -c /home/runner/work/marbellous/marbellous/backend/src/WGLContext.cpp

CMakeFiles/Marbellous.dir/src/WGLContext.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Marbellous.dir/src/WGLContext.cpp.i"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/marbellous/marbellous/backend/src/WGLContext.cpp > CMakeFiles/Marbellous.dir/src/WGLContext.cpp.i

CMakeFiles/Marbellous.dir/src/WGLContext.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Marbellous.dir/src/WGLContext.cpp.s"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/marbellous/marbellous/backend/src/WGLContext.cpp -o CMakeFiles/Marbellous.dir/src/WGLContext.cpp.s

CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.o: CMakeFiles/Marbellous.dir/flags.make
CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.o: CMakeFiles/Marbellous.dir/includes_CXX.rsp
CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.o: src/WGLRakeRenderer.cpp
CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.o: CMakeFiles/Marbellous.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/marbellous/marbellous/backend/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.o"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.o -MF CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.o.d -o CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.o -c /home/runner/work/marbellous/marbellous/backend/src/WGLRakeRenderer.cpp

CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.i"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/marbellous/marbellous/backend/src/WGLRakeRenderer.cpp > CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.i

CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.s"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/marbellous/marbellous/backend/src/WGLRakeRenderer.cpp -o CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.s

CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.o: CMakeFiles/Marbellous.dir/flags.make
CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.o: CMakeFiles/Marbellous.dir/includes_CXX.rsp
CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.o: src/WGLRenderer.cpp
CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.o: CMakeFiles/Marbellous.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/marbellous/marbellous/backend/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.o"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.o -MF CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.o.d -o CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.o -c /home/runner/work/marbellous/marbellous/backend/src/WGLRenderer.cpp

CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.i"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/marbellous/marbellous/backend/src/WGLRenderer.cpp > CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.i

CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.s"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/marbellous/marbellous/backend/src/WGLRenderer.cpp -o CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.s

CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.o: CMakeFiles/Marbellous.dir/flags.make
CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.o: CMakeFiles/Marbellous.dir/includes_CXX.rsp
CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.o: src/WGLSceneRenderer.cpp
CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.o: CMakeFiles/Marbellous.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/marbellous/marbellous/backend/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.o"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.o -MF CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.o.d -o CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.o -c /home/runner/work/marbellous/marbellous/backend/src/WGLSceneRenderer.cpp

CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.i"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/marbellous/marbellous/backend/src/WGLSceneRenderer.cpp > CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.i

CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.s"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/marbellous/marbellous/backend/src/WGLSceneRenderer.cpp -o CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.s

CMakeFiles/Marbellous.dir/src/main.cpp.o: CMakeFiles/Marbellous.dir/flags.make
CMakeFiles/Marbellous.dir/src/main.cpp.o: CMakeFiles/Marbellous.dir/includes_CXX.rsp
CMakeFiles/Marbellous.dir/src/main.cpp.o: src/main.cpp
CMakeFiles/Marbellous.dir/src/main.cpp.o: CMakeFiles/Marbellous.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/marbellous/marbellous/backend/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Marbellous.dir/src/main.cpp.o"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Marbellous.dir/src/main.cpp.o -MF CMakeFiles/Marbellous.dir/src/main.cpp.o.d -o CMakeFiles/Marbellous.dir/src/main.cpp.o -c /home/runner/work/marbellous/marbellous/backend/src/main.cpp

CMakeFiles/Marbellous.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Marbellous.dir/src/main.cpp.i"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/marbellous/marbellous/backend/src/main.cpp > CMakeFiles/Marbellous.dir/src/main.cpp.i

CMakeFiles/Marbellous.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Marbellous.dir/src/main.cpp.s"
	/home/runner/work/marbellous/marbellous/emsdk-cache/emsdk-master/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/marbellous/marbellous/backend/src/main.cpp -o CMakeFiles/Marbellous.dir/src/main.cpp.s

# Object files for target Marbellous
Marbellous_OBJECTS = \
"CMakeFiles/Marbellous.dir/src/Color.cpp.o" \
"CMakeFiles/Marbellous.dir/src/Options.cpp.o" \
"CMakeFiles/Marbellous.dir/src/Palette.cpp.o" \
"CMakeFiles/Marbellous.dir/src/Polygon.cpp.o" \
"CMakeFiles/Marbellous.dir/src/WGLContext.cpp.o" \
"CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.o" \
"CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.o" \
"CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.o" \
"CMakeFiles/Marbellous.dir/src/main.cpp.o"

# External object files for target Marbellous
Marbellous_EXTERNAL_OBJECTS =

Marbellous.js: CMakeFiles/Marbellous.dir/src/Color.cpp.o
Marbellous.js: CMakeFiles/Marbellous.dir/src/Options.cpp.o
Marbellous.js: CMakeFiles/Marbellous.dir/src/Palette.cpp.o
Marbellous.js: CMakeFiles/Marbellous.dir/src/Polygon.cpp.o
Marbellous.js: CMakeFiles/Marbellous.dir/src/WGLContext.cpp.o
Marbellous.js: CMakeFiles/Marbellous.dir/src/WGLRakeRenderer.cpp.o
Marbellous.js: CMakeFiles/Marbellous.dir/src/WGLRenderer.cpp.o
Marbellous.js: CMakeFiles/Marbellous.dir/src/WGLSceneRenderer.cpp.o
Marbellous.js: CMakeFiles/Marbellous.dir/src/main.cpp.o
Marbellous.js: CMakeFiles/Marbellous.dir/build.make
Marbellous.js: CMakeFiles/Marbellous.dir/objects1.rsp
Marbellous.js: CMakeFiles/Marbellous.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/runner/work/marbellous/marbellous/backend/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable Marbellous.js"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Marbellous.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Marbellous.dir/build: Marbellous.js
.PHONY : CMakeFiles/Marbellous.dir/build

CMakeFiles/Marbellous.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Marbellous.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Marbellous.dir/clean

CMakeFiles/Marbellous.dir/depend:
	cd /home/runner/work/marbellous/marbellous/backend && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/runner/work/marbellous/marbellous/backend /home/runner/work/marbellous/marbellous/backend /home/runner/work/marbellous/marbellous/backend /home/runner/work/marbellous/marbellous/backend /home/runner/work/marbellous/marbellous/backend/CMakeFiles/Marbellous.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Marbellous.dir/depend

