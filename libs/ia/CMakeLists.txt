cmake_minimum_required(VERSION 3.16)
project(ia
	DESCRIPTION "ia is a library for interval arithmetics"
	VERSION 0.0.1
	LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

if(NOT util_SOURCE_DIR)
	find_package(util REQUIRED)
endif()

add_library(ia
	include/ia/operations.hpp
	include/ia/interval.hpp
	include/ia/scoped-round-mode.hpp

	src/scoped-round-mode.cpp)

add_library(ia::ia ALIAS ia)

target_include_directories(
	ia PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_link_libraries(ia PUBLIC util::util)

target_compile_features(ia PUBLIC cxx_std_20)

if(ENABLE_TESTING)
	find_package(Catch2 REQUIRED)
	enable_testing()
	add_executable(
		ia-tests
		tests/scoped-round-mode.cpp
		tests/interval.cpp)

	target_link_libraries(ia-tests ia Catch2::Catch2)

	include(CTest)
	include(Catch)
	catch_discover_tests(ia-tests TEST_PREFIX "ia-")
endif()

if(ENABLE_PCH)
	target_precompile_headers(
		project_options
		INTERFACE
		include/ia/operations.hpp
		include/ia/interval.hpp
		include/ia/scoped-round-mode.hpp)
endif()

set_property(TARGET ia PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET ia PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ia
	APPEND PROPERTY
	COMPATIBLE_INTERFACE_STRING ${PROJECT_VERSION_MAJOR})

install(TARGETS ia EXPORT iaTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include)

install(EXPORT iaTargets
	FILE iaTargets.cmake
	NAMESPACE ia::
	DESTINATION lib/cmake/ia)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/iaConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion)

install(FILES iaConfig.cmake iaConfigVersion.cmake
	DESTINATION lib/cmake/ia)
