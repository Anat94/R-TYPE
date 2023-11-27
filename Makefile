# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anatole/delivery/Tek3/R-TYPE

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anatole/delivery/Tek3/R-TYPE

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/anatole/delivery/Tek3/R-TYPE/CMakeFiles /home/anatole/delivery/Tek3/R-TYPE//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/anatole/delivery/Tek3/R-TYPE/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named server

# Build rule for target.
server: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 server
.PHONY : server

# fast build rule for target.
server/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/build
.PHONY : server/fast

#=============================================================================
# Target rules for targets named client

# Build rule for target.
client: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 client
.PHONY : client

# fast build rule for target.
client/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/client.dir/build.make CMakeFiles/client.dir/build
.PHONY : client/fast

#=============================================================================
# Target rules for targets named ecs

# Build rule for target.
ecs: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 ecs
.PHONY : ecs

# fast build rule for target.
ecs/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/build
.PHONY : ecs/fast

src/Client/main.o: src/Client/main.cpp.o
.PHONY : src/Client/main.o

# target to build an object file
src/Client/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/client.dir/build.make CMakeFiles/client.dir/src/Client/main.cpp.o
.PHONY : src/Client/main.cpp.o

src/Client/main.i: src/Client/main.cpp.i
.PHONY : src/Client/main.i

# target to preprocess a source file
src/Client/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/client.dir/build.make CMakeFiles/client.dir/src/Client/main.cpp.i
.PHONY : src/Client/main.cpp.i

src/Client/main.s: src/Client/main.cpp.s
.PHONY : src/Client/main.s

# target to generate assembly for a file
src/Client/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/client.dir/build.make CMakeFiles/client.dir/src/Client/main.cpp.s
.PHONY : src/Client/main.cpp.s

src/Ecs/main.o: src/Ecs/main.cpp.o
.PHONY : src/Ecs/main.o

# target to build an object file
src/Ecs/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/src/Ecs/main.cpp.o
.PHONY : src/Ecs/main.cpp.o

src/Ecs/main.i: src/Ecs/main.cpp.i
.PHONY : src/Ecs/main.i

# target to preprocess a source file
src/Ecs/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/src/Ecs/main.cpp.i
.PHONY : src/Ecs/main.cpp.i

src/Ecs/main.s: src/Ecs/main.cpp.s
.PHONY : src/Ecs/main.s

# target to generate assembly for a file
src/Ecs/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/src/Ecs/main.cpp.s
.PHONY : src/Ecs/main.cpp.s

src/Server/main.o: src/Server/main.cpp.o
.PHONY : src/Server/main.o

# target to build an object file
src/Server/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/src/Server/main.cpp.o
.PHONY : src/Server/main.cpp.o

src/Server/main.i: src/Server/main.cpp.i
.PHONY : src/Server/main.i

# target to preprocess a source file
src/Server/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/src/Server/main.cpp.i
.PHONY : src/Server/main.cpp.i

src/Server/main.s: src/Server/main.cpp.s
.PHONY : src/Server/main.s

# target to generate assembly for a file
src/Server/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/src/Server/main.cpp.s
.PHONY : src/Server/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... client"
	@echo "... ecs"
	@echo "... server"
	@echo "... src/Client/main.o"
	@echo "... src/Client/main.i"
	@echo "... src/Client/main.s"
	@echo "... src/Ecs/main.o"
	@echo "... src/Ecs/main.i"
	@echo "... src/Ecs/main.s"
	@echo "... src/Server/main.o"
	@echo "... src/Server/main.i"
	@echo "... src/Server/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

