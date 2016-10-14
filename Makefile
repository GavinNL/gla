# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/gavin/Code/GL_projects/gla

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gavin/Code/GL_projects/gla

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/cmake-gui -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/gavin/Code/GL_projects/gla/CMakeFiles /home/gavin/Code/GL_projects/gla/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/gavin/Code/GL_projects/gla/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named 01_HelloTriangle

# Build rule for target.
01_HelloTriangle: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 01_HelloTriangle
.PHONY : 01_HelloTriangle

# fast build rule for target.
01_HelloTriangle/fast:
	$(MAKE) -f CMakeFiles/01_HelloTriangle.dir/build.make CMakeFiles/01_HelloTriangle.dir/build
.PHONY : 01_HelloTriangle/fast

#=============================================================================
# Target rules for targets named 05_Sampler2DArray

# Build rule for target.
05_Sampler2DArray: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 05_Sampler2DArray
.PHONY : 05_Sampler2DArray

# fast build rule for target.
05_Sampler2DArray/fast:
	$(MAKE) -f CMakeFiles/05_Sampler2DArray.dir/build.make CMakeFiles/05_Sampler2DArray.dir/build
.PHONY : 05_Sampler2DArray/fast

#=============================================================================
# Target rules for targets named other

# Build rule for target.
other: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 other
.PHONY : other

# fast build rule for target.
other/fast:
	$(MAKE) -f CMakeFiles/other.dir/build.make CMakeFiles/other.dir/build
.PHONY : other/fast

#=============================================================================
# Target rules for targets named resources

# Build rule for target.
resources: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 resources
.PHONY : resources

# fast build rule for target.
resources/fast:
	$(MAKE) -f CMakeFiles/resources.dir/build.make CMakeFiles/resources.dir/build
.PHONY : resources/fast

#=============================================================================
# Target rules for targets named headers

# Build rule for target.
headers: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 headers
.PHONY : headers

# fast build rule for target.
headers/fast:
	$(MAKE) -f CMakeFiles/headers.dir/build.make CMakeFiles/headers.dir/build
.PHONY : headers/fast

#=============================================================================
# Target rules for targets named 02_HelloTriangleIndexed

# Build rule for target.
02_HelloTriangleIndexed: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 02_HelloTriangleIndexed
.PHONY : 02_HelloTriangleIndexed

# fast build rule for target.
02_HelloTriangleIndexed/fast:
	$(MAKE) -f CMakeFiles/02_HelloTriangleIndexed.dir/build.make CMakeFiles/02_HelloTriangleIndexed.dir/build
.PHONY : 02_HelloTriangleIndexed/fast

#=============================================================================
# Target rules for targets named 03_HelloTriangleVertexArray

# Build rule for target.
03_HelloTriangleVertexArray: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 03_HelloTriangleVertexArray
.PHONY : 03_HelloTriangleVertexArray

# fast build rule for target.
03_HelloTriangleVertexArray/fast:
	$(MAKE) -f CMakeFiles/03_HelloTriangleVertexArray.dir/build.make CMakeFiles/03_HelloTriangleVertexArray.dir/build
.PHONY : 03_HelloTriangleVertexArray/fast

#=============================================================================
# Target rules for targets named 07_FrameBuffer

# Build rule for target.
07_FrameBuffer: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 07_FrameBuffer
.PHONY : 07_FrameBuffer

# fast build rule for target.
07_FrameBuffer/fast:
	$(MAKE) -f CMakeFiles/07_FrameBuffer.dir/build.make CMakeFiles/07_FrameBuffer.dir/build
.PHONY : 07_FrameBuffer/fast

#=============================================================================
# Target rules for targets named 04_HelloTexturedTriangle

# Build rule for target.
04_HelloTexturedTriangle: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 04_HelloTexturedTriangle
.PHONY : 04_HelloTexturedTriangle

# fast build rule for target.
04_HelloTexturedTriangle/fast:
	$(MAKE) -f CMakeFiles/04_HelloTexturedTriangle.dir/build.make CMakeFiles/04_HelloTexturedTriangle.dir/build
.PHONY : 04_HelloTexturedTriangle/fast

#=============================================================================
# Target rules for targets named 06_UniformBuffer

# Build rule for target.
06_UniformBuffer: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 06_UniformBuffer
.PHONY : 06_UniformBuffer

# fast build rule for target.
06_UniformBuffer/fast:
	$(MAKE) -f CMakeFiles/06_UniformBuffer.dir/build.make CMakeFiles/06_UniformBuffer.dir/build
.PHONY : 06_UniformBuffer/fast

examples/01_HelloTriangle.o: examples/01_HelloTriangle.cpp.o

.PHONY : examples/01_HelloTriangle.o

# target to build an object file
examples/01_HelloTriangle.cpp.o:
	$(MAKE) -f CMakeFiles/01_HelloTriangle.dir/build.make CMakeFiles/01_HelloTriangle.dir/examples/01_HelloTriangle.cpp.o
.PHONY : examples/01_HelloTriangle.cpp.o

examples/01_HelloTriangle.i: examples/01_HelloTriangle.cpp.i

.PHONY : examples/01_HelloTriangle.i

# target to preprocess a source file
examples/01_HelloTriangle.cpp.i:
	$(MAKE) -f CMakeFiles/01_HelloTriangle.dir/build.make CMakeFiles/01_HelloTriangle.dir/examples/01_HelloTriangle.cpp.i
.PHONY : examples/01_HelloTriangle.cpp.i

examples/01_HelloTriangle.s: examples/01_HelloTriangle.cpp.s

.PHONY : examples/01_HelloTriangle.s

# target to generate assembly for a file
examples/01_HelloTriangle.cpp.s:
	$(MAKE) -f CMakeFiles/01_HelloTriangle.dir/build.make CMakeFiles/01_HelloTriangle.dir/examples/01_HelloTriangle.cpp.s
.PHONY : examples/01_HelloTriangle.cpp.s

examples/02_HelloTriangleIndexed.o: examples/02_HelloTriangleIndexed.cpp.o

.PHONY : examples/02_HelloTriangleIndexed.o

# target to build an object file
examples/02_HelloTriangleIndexed.cpp.o:
	$(MAKE) -f CMakeFiles/02_HelloTriangleIndexed.dir/build.make CMakeFiles/02_HelloTriangleIndexed.dir/examples/02_HelloTriangleIndexed.cpp.o
.PHONY : examples/02_HelloTriangleIndexed.cpp.o

examples/02_HelloTriangleIndexed.i: examples/02_HelloTriangleIndexed.cpp.i

.PHONY : examples/02_HelloTriangleIndexed.i

# target to preprocess a source file
examples/02_HelloTriangleIndexed.cpp.i:
	$(MAKE) -f CMakeFiles/02_HelloTriangleIndexed.dir/build.make CMakeFiles/02_HelloTriangleIndexed.dir/examples/02_HelloTriangleIndexed.cpp.i
.PHONY : examples/02_HelloTriangleIndexed.cpp.i

examples/02_HelloTriangleIndexed.s: examples/02_HelloTriangleIndexed.cpp.s

.PHONY : examples/02_HelloTriangleIndexed.s

# target to generate assembly for a file
examples/02_HelloTriangleIndexed.cpp.s:
	$(MAKE) -f CMakeFiles/02_HelloTriangleIndexed.dir/build.make CMakeFiles/02_HelloTriangleIndexed.dir/examples/02_HelloTriangleIndexed.cpp.s
.PHONY : examples/02_HelloTriangleIndexed.cpp.s

examples/03_HelloTriangleVertexArray.o: examples/03_HelloTriangleVertexArray.cpp.o

.PHONY : examples/03_HelloTriangleVertexArray.o

# target to build an object file
examples/03_HelloTriangleVertexArray.cpp.o:
	$(MAKE) -f CMakeFiles/03_HelloTriangleVertexArray.dir/build.make CMakeFiles/03_HelloTriangleVertexArray.dir/examples/03_HelloTriangleVertexArray.cpp.o
.PHONY : examples/03_HelloTriangleVertexArray.cpp.o

examples/03_HelloTriangleVertexArray.i: examples/03_HelloTriangleVertexArray.cpp.i

.PHONY : examples/03_HelloTriangleVertexArray.i

# target to preprocess a source file
examples/03_HelloTriangleVertexArray.cpp.i:
	$(MAKE) -f CMakeFiles/03_HelloTriangleVertexArray.dir/build.make CMakeFiles/03_HelloTriangleVertexArray.dir/examples/03_HelloTriangleVertexArray.cpp.i
.PHONY : examples/03_HelloTriangleVertexArray.cpp.i

examples/03_HelloTriangleVertexArray.s: examples/03_HelloTriangleVertexArray.cpp.s

.PHONY : examples/03_HelloTriangleVertexArray.s

# target to generate assembly for a file
examples/03_HelloTriangleVertexArray.cpp.s:
	$(MAKE) -f CMakeFiles/03_HelloTriangleVertexArray.dir/build.make CMakeFiles/03_HelloTriangleVertexArray.dir/examples/03_HelloTriangleVertexArray.cpp.s
.PHONY : examples/03_HelloTriangleVertexArray.cpp.s

examples/04_HelloTexturedTriangle.o: examples/04_HelloTexturedTriangle.cpp.o

.PHONY : examples/04_HelloTexturedTriangle.o

# target to build an object file
examples/04_HelloTexturedTriangle.cpp.o:
	$(MAKE) -f CMakeFiles/04_HelloTexturedTriangle.dir/build.make CMakeFiles/04_HelloTexturedTriangle.dir/examples/04_HelloTexturedTriangle.cpp.o
.PHONY : examples/04_HelloTexturedTriangle.cpp.o

examples/04_HelloTexturedTriangle.i: examples/04_HelloTexturedTriangle.cpp.i

.PHONY : examples/04_HelloTexturedTriangle.i

# target to preprocess a source file
examples/04_HelloTexturedTriangle.cpp.i:
	$(MAKE) -f CMakeFiles/04_HelloTexturedTriangle.dir/build.make CMakeFiles/04_HelloTexturedTriangle.dir/examples/04_HelloTexturedTriangle.cpp.i
.PHONY : examples/04_HelloTexturedTriangle.cpp.i

examples/04_HelloTexturedTriangle.s: examples/04_HelloTexturedTriangle.cpp.s

.PHONY : examples/04_HelloTexturedTriangle.s

# target to generate assembly for a file
examples/04_HelloTexturedTriangle.cpp.s:
	$(MAKE) -f CMakeFiles/04_HelloTexturedTriangle.dir/build.make CMakeFiles/04_HelloTexturedTriangle.dir/examples/04_HelloTexturedTriangle.cpp.s
.PHONY : examples/04_HelloTexturedTriangle.cpp.s

examples/05_Sampler2DArray.o: examples/05_Sampler2DArray.cpp.o

.PHONY : examples/05_Sampler2DArray.o

# target to build an object file
examples/05_Sampler2DArray.cpp.o:
	$(MAKE) -f CMakeFiles/05_Sampler2DArray.dir/build.make CMakeFiles/05_Sampler2DArray.dir/examples/05_Sampler2DArray.cpp.o
.PHONY : examples/05_Sampler2DArray.cpp.o

examples/05_Sampler2DArray.i: examples/05_Sampler2DArray.cpp.i

.PHONY : examples/05_Sampler2DArray.i

# target to preprocess a source file
examples/05_Sampler2DArray.cpp.i:
	$(MAKE) -f CMakeFiles/05_Sampler2DArray.dir/build.make CMakeFiles/05_Sampler2DArray.dir/examples/05_Sampler2DArray.cpp.i
.PHONY : examples/05_Sampler2DArray.cpp.i

examples/05_Sampler2DArray.s: examples/05_Sampler2DArray.cpp.s

.PHONY : examples/05_Sampler2DArray.s

# target to generate assembly for a file
examples/05_Sampler2DArray.cpp.s:
	$(MAKE) -f CMakeFiles/05_Sampler2DArray.dir/build.make CMakeFiles/05_Sampler2DArray.dir/examples/05_Sampler2DArray.cpp.s
.PHONY : examples/05_Sampler2DArray.cpp.s

examples/06_UniformBuffer.o: examples/06_UniformBuffer.cpp.o

.PHONY : examples/06_UniformBuffer.o

# target to build an object file
examples/06_UniformBuffer.cpp.o:
	$(MAKE) -f CMakeFiles/06_UniformBuffer.dir/build.make CMakeFiles/06_UniformBuffer.dir/examples/06_UniformBuffer.cpp.o
.PHONY : examples/06_UniformBuffer.cpp.o

examples/06_UniformBuffer.i: examples/06_UniformBuffer.cpp.i

.PHONY : examples/06_UniformBuffer.i

# target to preprocess a source file
examples/06_UniformBuffer.cpp.i:
	$(MAKE) -f CMakeFiles/06_UniformBuffer.dir/build.make CMakeFiles/06_UniformBuffer.dir/examples/06_UniformBuffer.cpp.i
.PHONY : examples/06_UniformBuffer.cpp.i

examples/06_UniformBuffer.s: examples/06_UniformBuffer.cpp.s

.PHONY : examples/06_UniformBuffer.s

# target to generate assembly for a file
examples/06_UniformBuffer.cpp.s:
	$(MAKE) -f CMakeFiles/06_UniformBuffer.dir/build.make CMakeFiles/06_UniformBuffer.dir/examples/06_UniformBuffer.cpp.s
.PHONY : examples/06_UniformBuffer.cpp.s

examples/07_FrameBuffer.o: examples/07_FrameBuffer.cpp.o

.PHONY : examples/07_FrameBuffer.o

# target to build an object file
examples/07_FrameBuffer.cpp.o:
	$(MAKE) -f CMakeFiles/07_FrameBuffer.dir/build.make CMakeFiles/07_FrameBuffer.dir/examples/07_FrameBuffer.cpp.o
.PHONY : examples/07_FrameBuffer.cpp.o

examples/07_FrameBuffer.i: examples/07_FrameBuffer.cpp.i

.PHONY : examples/07_FrameBuffer.i

# target to preprocess a source file
examples/07_FrameBuffer.cpp.i:
	$(MAKE) -f CMakeFiles/07_FrameBuffer.dir/build.make CMakeFiles/07_FrameBuffer.dir/examples/07_FrameBuffer.cpp.i
.PHONY : examples/07_FrameBuffer.cpp.i

examples/07_FrameBuffer.s: examples/07_FrameBuffer.cpp.s

.PHONY : examples/07_FrameBuffer.s

# target to generate assembly for a file
examples/07_FrameBuffer.cpp.s:
	$(MAKE) -f CMakeFiles/07_FrameBuffer.dir/build.make CMakeFiles/07_FrameBuffer.dir/examples/07_FrameBuffer.cpp.s
.PHONY : examples/07_FrameBuffer.cpp.s

glad.o: glad.cpp.o

.PHONY : glad.o

# target to build an object file
glad.cpp.o:
	$(MAKE) -f CMakeFiles/01_HelloTriangle.dir/build.make CMakeFiles/01_HelloTriangle.dir/glad.cpp.o
	$(MAKE) -f CMakeFiles/05_Sampler2DArray.dir/build.make CMakeFiles/05_Sampler2DArray.dir/glad.cpp.o
	$(MAKE) -f CMakeFiles/02_HelloTriangleIndexed.dir/build.make CMakeFiles/02_HelloTriangleIndexed.dir/glad.cpp.o
	$(MAKE) -f CMakeFiles/03_HelloTriangleVertexArray.dir/build.make CMakeFiles/03_HelloTriangleVertexArray.dir/glad.cpp.o
	$(MAKE) -f CMakeFiles/07_FrameBuffer.dir/build.make CMakeFiles/07_FrameBuffer.dir/glad.cpp.o
	$(MAKE) -f CMakeFiles/04_HelloTexturedTriangle.dir/build.make CMakeFiles/04_HelloTexturedTriangle.dir/glad.cpp.o
	$(MAKE) -f CMakeFiles/06_UniformBuffer.dir/build.make CMakeFiles/06_UniformBuffer.dir/glad.cpp.o
.PHONY : glad.cpp.o

glad.i: glad.cpp.i

.PHONY : glad.i

# target to preprocess a source file
glad.cpp.i:
	$(MAKE) -f CMakeFiles/01_HelloTriangle.dir/build.make CMakeFiles/01_HelloTriangle.dir/glad.cpp.i
	$(MAKE) -f CMakeFiles/05_Sampler2DArray.dir/build.make CMakeFiles/05_Sampler2DArray.dir/glad.cpp.i
	$(MAKE) -f CMakeFiles/02_HelloTriangleIndexed.dir/build.make CMakeFiles/02_HelloTriangleIndexed.dir/glad.cpp.i
	$(MAKE) -f CMakeFiles/03_HelloTriangleVertexArray.dir/build.make CMakeFiles/03_HelloTriangleVertexArray.dir/glad.cpp.i
	$(MAKE) -f CMakeFiles/07_FrameBuffer.dir/build.make CMakeFiles/07_FrameBuffer.dir/glad.cpp.i
	$(MAKE) -f CMakeFiles/04_HelloTexturedTriangle.dir/build.make CMakeFiles/04_HelloTexturedTriangle.dir/glad.cpp.i
	$(MAKE) -f CMakeFiles/06_UniformBuffer.dir/build.make CMakeFiles/06_UniformBuffer.dir/glad.cpp.i
.PHONY : glad.cpp.i

glad.s: glad.cpp.s

.PHONY : glad.s

# target to generate assembly for a file
glad.cpp.s:
	$(MAKE) -f CMakeFiles/01_HelloTriangle.dir/build.make CMakeFiles/01_HelloTriangle.dir/glad.cpp.s
	$(MAKE) -f CMakeFiles/05_Sampler2DArray.dir/build.make CMakeFiles/05_Sampler2DArray.dir/glad.cpp.s
	$(MAKE) -f CMakeFiles/02_HelloTriangleIndexed.dir/build.make CMakeFiles/02_HelloTriangleIndexed.dir/glad.cpp.s
	$(MAKE) -f CMakeFiles/03_HelloTriangleVertexArray.dir/build.make CMakeFiles/03_HelloTriangleVertexArray.dir/glad.cpp.s
	$(MAKE) -f CMakeFiles/07_FrameBuffer.dir/build.make CMakeFiles/07_FrameBuffer.dir/glad.cpp.s
	$(MAKE) -f CMakeFiles/04_HelloTexturedTriangle.dir/build.make CMakeFiles/04_HelloTexturedTriangle.dir/glad.cpp.s
	$(MAKE) -f CMakeFiles/06_UniformBuffer.dir/build.make CMakeFiles/06_UniformBuffer.dir/glad.cpp.s
.PHONY : glad.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... 01_HelloTriangle"
	@echo "... 05_Sampler2DArray"
	@echo "... other"
	@echo "... edit_cache"
	@echo "... resources"
	@echo "... headers"
	@echo "... 02_HelloTriangleIndexed"
	@echo "... 03_HelloTriangleVertexArray"
	@echo "... 07_FrameBuffer"
	@echo "... 04_HelloTexturedTriangle"
	@echo "... 06_UniformBuffer"
	@echo "... examples/01_HelloTriangle.o"
	@echo "... examples/01_HelloTriangle.i"
	@echo "... examples/01_HelloTriangle.s"
	@echo "... examples/02_HelloTriangleIndexed.o"
	@echo "... examples/02_HelloTriangleIndexed.i"
	@echo "... examples/02_HelloTriangleIndexed.s"
	@echo "... examples/03_HelloTriangleVertexArray.o"
	@echo "... examples/03_HelloTriangleVertexArray.i"
	@echo "... examples/03_HelloTriangleVertexArray.s"
	@echo "... examples/04_HelloTexturedTriangle.o"
	@echo "... examples/04_HelloTexturedTriangle.i"
	@echo "... examples/04_HelloTexturedTriangle.s"
	@echo "... examples/05_Sampler2DArray.o"
	@echo "... examples/05_Sampler2DArray.i"
	@echo "... examples/05_Sampler2DArray.s"
	@echo "... examples/06_UniformBuffer.o"
	@echo "... examples/06_UniformBuffer.i"
	@echo "... examples/06_UniformBuffer.s"
	@echo "... examples/07_FrameBuffer.o"
	@echo "... examples/07_FrameBuffer.i"
	@echo "... examples/07_FrameBuffer.s"
	@echo "... glad.o"
	@echo "... glad.i"
	@echo "... glad.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

