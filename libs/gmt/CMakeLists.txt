
cmake_minimum_required(VERSION 3.23)
project(gmt
	DESCRIPTION "gmt is a simple geometry library"
	VERSION 0.0.1
	LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

add_library(gmt INTERFACE)
add_library(gmt::gmt ALIAS gmt)

target_include_directories(
	gmt INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_sources(gmt PUBLIC FILE_SET HEADERS
	BASE_DIRS "./include"
	FILES
		./include/gmt/mat-inversion.hpp
		./include/gmt/mat.hpp
		./include/gmt/embedded-point.hpp
		./include/gmt/point-operations.hpp
		./include/gmt/sphere.hpp
		./include/gmt/point.hpp
		./include/gmt/distance.hpp
		./include/gmt/cross-product.hpp
		./include/gmt/point-promotion.hpp
		./include/gmt/vector.hpp)

target_link_libraries(gmt INTERFACE)

target_compile_features(gmt INTERFACE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(gmt INTERFACE
		-fexperimental-library
		-stdlib=libc++
	)

	target_link_options(gmt INTERFACE
		-fexperimental-library
		-stdlib=libc++
		-lc++abi
	)
endif()

if(ENABLE_TESTING)
	if(NOT util_SOURCE_DIR)
		find_package(util REQUIRED)
	endif()
	find_package(Catch2 REQUIRED)

	enable_testing()
	add_executable(
		gmt-tests
		./tests/point.cpp
		./tests/mat.cpp
		./tests/mat-inversion.cpp
		./tests/embedded-point.cpp
		./tests/cross-product.cpp)

	target_link_libraries(gmt-tests gmt util::util Catch2::Catch2 Catch2::Catch2WithMain)

	include(CTest)
	include(Catch)
	catch_discover_tests(gmt-tests TEST_PREFIX "gmt-")
endif()

if(ENABLE_PCH)
	target_precompile_headers(
		project_options
		INTERFACE
		./include/gmt/mat.hpp
		./include/gmt/mat-inversion.hpp
		./include/gmt/point-operations.hpp
		./include/gmt/sphere.hpp
		./include/gmt/point.hpp
		./include/gmt/distance.hpp
		./include/gmt/cross-product.hpp
		./include/gmt/point-promotion.hpp
		./include/gmt/vector.hpp
)
endif()

set_property(TARGET gmt PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET gmt PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET gmt
	APPEND PROPERTY
	COMPATIBLE_INTERFACE_STRING ${PROJECT_VERSION_MAJOR})

install(TARGETS gmt EXPORT ug-gmt-targets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
	FILE_SET HEADERS)

install(EXPORT ug-gmt-targets
	FILE ug-gmt-targets.cmake
	NAMESPACE ug::
	DESTINATION lib/cmake/ug/gmt)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/ug-gmt-config-version.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion)

configure_package_config_file(
	cmake/config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/ug-gmt-config.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/ug/gmt/cmake"
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/ug-gmt-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/ug-gmt-config-version.cmake"
	DESTINATION lib/cmake/ug/gmt)
