cmake_minimum_required(VERSION 3.16)
project(containers
	DESCRIPTION "containers is a library of containers; the aim is not to substitute the std:: containers but tto complement it"
	VERSION 0.0.1
	LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

add_library(containers INTERFACE)

add_library(containers::containers ALIAS containers)

if(NOT util_SOURCE_DIR)
	find_package(util REQUIRED)
endif()

target_include_directories(
	containers INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_compile_features(containers INTERFACE cxx_std_20)

if(ENABLE_TESTING)
	find_package(Catch2 REQUIRED)
	enable_testing()
	add_executable(
		containers-tests
		tests/containers-tests.cpp
		tests/run-length-container.cpp)

	target_link_libraries(containers-tests containers util::util)

	include(CTest)
	include(Catch)
	catch_discover_tests(containers-tests TEST_PREFIX "containers-")
endif()

if(ENABLE_PCH)
	target_precompile_headers(
		project_options
		INTERFACE
		include/containers/run-length-container.hpp
)
endif()

set_property(TARGET containers PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET containers PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET containers
	APPEND PROPERTY
	COMPATIBLE_INTERFACE_STRING ${PROJECT_VERSION_MAJOR})

install(TARGETS containers EXPORT containersTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include)

install(EXPORT containersTargets
	FILE containersTargets.cmake
	NAMESPACE containers::
	DESTINATION lib/cmake/containers)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/containersConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion)

install(FILES containersConfig.cmake containersConfigVersion.cmake
	DESTINATION lib/cmake/containers)
