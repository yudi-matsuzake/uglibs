cmake_minimum_required(VERSION 3.23)
project(containers
	DESCRIPTION "containers is a library of containers; the aim is not to substitute the std:: containers but to complement it"
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

target_sources(containers PUBLIC FILE_SET HEADERS
	BASE_DIRS "./include"
	FILES
		./include/containers/run-length-container.hpp
		./include/containers/ring-hash-map.hpp
		./include/containers/ring-vector.hpp
		./include/containers/bitset.hpp
		./include/containers/tight-integers-container.hpp
		./include/containers/bit-container-adaptor.hpp)


target_compile_features(containers INTERFACE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(containers INTERFACE
		-fexperimental-library
		-stdlib=libc++
	)

	target_link_options(containers INTERFACE
		-fexperimental-library
		-stdlib=libc++
		-lc++abi
	)
endif()

if(ENABLE_TESTING)
	find_package(Catch2 REQUIRED)
	enable_testing()
	add_executable(
		containers-tests
		tests/bit-container-adaptor.cpp
		tests/tight-integer-container.cpp
		tests/ring-vector.cpp
		tests/ring-hash-map.cpp
		tests/bitset.cpp
		tests/run-length-container.cpp)

	target_link_libraries(containers-tests Catch2::Catch2 Catch2::Catch2WithMain containers util::util)

	include(CTest)
	include(Catch)
	catch_discover_tests(containers-tests TEST_PREFIX "containers-")
endif()

if(ENABLE_PCH)
	target_precompile_headers(
		project_options
		INTERFACE
		include/containers/run-length-container.hpp
		include/containers/run-length-container.hpp
)
endif()

set_property(TARGET containers PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET containers PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET containers
	APPEND PROPERTY
	COMPATIBLE_INTERFACE_STRING ${PROJECT_VERSION_MAJOR})

install(TARGETS containers EXPORT ug-containers-targets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
	FILE_SET HEADERS)

install(EXPORT ug-containers-targets
	FILE ug-containers-targets.cmake
	NAMESPACE ug::
	DESTINATION lib/cmake/ug/containers)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/ug-containers-config-version.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion)

configure_package_config_file(
	cmake/config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/ug-containers-config.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/ug/containers/cmake"
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/ug-containers-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/ug-containers-config-version.cmake"
	DESTINATION lib/cmake/ug/containers)
