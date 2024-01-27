cmake_minimum_required(VERSION 3.14)

project(uglibs
	VERSION 0.0.1
	DESCRIPTION "uglibs is a collection of c++ libraries"
	LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

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
