#
# Impulse Engine CMake Configuration File
#
# Note: Example building and test are OFF by default
#       simply because they do not yet exist. This will
#       be updated as soon as we have tests and examples!
#

cmake_minimum_required(VERSION 3.20)
project(
	ImpulseEngine
	VERSION 0.0.1
	LANGUAGES CXX
)

# Global settings, the usual stuff
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)	
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Build options (there will be a lot of these)
option(IM_BUILD_TESTS "Build all tests." OFF)
option(IM_BUILD_EXAMPLES "Build all examples." OFF)
option(IM_PEDANTIC_COMPILING "Enables highest level of compilation warnings." ON)

if(IM_PEDANTIC_COMPILING)
	if(MSVC)
		add_compile_options(/W4 /permissive-)
	else()
		add_compile_options(-Wall -Wextra -Wpedantic)
	endif()
endif()

# Engine Core
add_library(
	impulse_core
	Source/Runtime.cpp
	Source/Logger.cpp
)

add_library(impulse::core ALIAS impulse_core)

target_include_directories(
	impulse_core
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/Include
)

# Examples
if(IM_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

# Tests
if(IM_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
