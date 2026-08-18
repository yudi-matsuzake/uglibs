cmake_minimum_required(VERSION 3.23)

project(uglibs
	VERSION 0.0.1
	DESCRIPTION "uglibs is a collection of c++ libraries"
	LANGUAGES C CXX
)

# C++ standard - owned by Nix; fallback for bare cmake invocations.
if(NOT DEFINED CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 23)
endif()
if(NOT DEFINED CMAKE_CXX_STANDARD_REQUIRED)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

# Copies imgui backends from $IMGUI_DIR into build dir. No-op without Nix.
include("${CMAKE_SOURCE_DIR}/cmake/nix-toolchain.cmake" OPTIONAL RESULT_VARIABLE _nix_toolchain)

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

option(ENABLE_TESTING "enable test builds" ON)

if(ENABLE_TESTING)
	find_package(Catch2 REQUIRED)
	enable_testing()
	include(CTest)
	include(Catch)
endif()

add_subdirectory(./libs/util)
add_subdirectory(./libs/gmt)
add_subdirectory(./libs/ia)
add_subdirectory(./libs/containers)
add_subdirectory(./libs/graphics)
