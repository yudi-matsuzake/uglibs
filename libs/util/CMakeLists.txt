cmake_minimum_required(VERSION 3.16)
project(util DESCRIPTION "util c++ library" VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

add_library(util
	./include/util/run-length-container.hpp
	./include/util/io.hpp
	./include/util/misc.hpp
	./include/util/container-operations.hpp
	./include/util/misc-adaptors.hpp
	./include/util/unsigned-integer.hpp
	./include/util/concepts.hpp
	./include/util/cv-adaptors.hpp
	./include/util/static-vector.hpp
	./src/misc.cpp
	./src/io.cpp)

add_library(util::util ALIAS util)

target_include_directories(
	util PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_compile_features(util PUBLIC cxx_std_20)

if(ENABLE_TESTING)
	find_package(Catch2 REQUIRED)
	enable_testing()
	add_executable(
		util-tests
		./tests/util-tests.cpp
		./tests/merged-variant.cpp
		./tests/misc-adaptors.cpp
		./tests/run-length-container.cpp)

	target_link_libraries(util-tests util)

	include(CTest)
	include(Catch)
	catch_discover_tests(util-tests)
endif()

if(ENABLE_PCH)
	target_precompile_headers(
		project_options
		INTERFACE
		./include/util/run-length-container.hpp
		./include/util/io.hpp
		./include/util/misc.hpp
		./include/util/container-operations.hpp
		./include/util/misc-adaptors.hpp
		./include/util/unsigned-integer.hpp
		./include/util/concepts.hpp
		./include/util/cv-adaptors.hpp
		./include/util/static-vector.hpp)
endif()

set_property(TARGET util PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET util PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET util
	APPEND PROPERTY
	COMPATIBLE_INTERFACE_STRING ${PROJECT_VERSION_MAJOR})

install(TARGETS util EXPORT utilTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include)

install(EXPORT utilTargets
	FILE utilTargets.cmake
	NAMESPACE util::
	DESTINATION lib/cmake/util)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/utilConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion)

install(FILES utilConfig.cmake utilConfigVersion.cmake
	DESTINATION lib/cmake/util)
