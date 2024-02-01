cmake_minimum_required(VERSION 3.16)
project(util DESCRIPTION "util c++ library" VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

find_package(range-v3 0.12.0 REQUIRED)
find_package(spdlog 1.12.0 REQUIRED)
find_package(fmt 10.1.1 REQUIRED)

add_library(util ./src/misc.cpp ./src/io.cpp)

add_library(util::util ALIAS util)
target_link_libraries(
	util PUBLIC range-v3::range-v3 spdlog::spdlog fmt::fmt)

target_include_directories(
	util
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_sources(util PUBLIC FILE_SET HEADERS
	BASE_DIRS "./include"
	FILES
		./include/util/variant.hpp
		./include/util/io.hpp
		./include/util/misc.hpp
		./include/util/container-operations.hpp
		./include/util/integers.hpp
		./include/util/underlying-integer.hpp
		./include/util/arithmetic-friends.hpp
		./include/util/meta.hpp
		./include/util/concepts.hpp
		./include/util/integer-variant.hpp
		./include/util/integer-concepts.hpp
		./include/util/cv-adaptors.hpp
		./include/util/views.hpp
		./include/util/ranges.hpp
		./include/util/log.hpp
		./include/util/bit-adaptors.hpp
		./include/util/bit.hpp
		./include/util/numeric-computing.hpp
		./include/util/static-string.hpp
		./include/util/static-vector.hpp
		./include/util/fmt.hpp)

target_compile_features(util PUBLIC cxx_std_23)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(util PUBLIC
		-fexperimental-library
		-stdlib=libc++
	)

	target_link_options(util PUBLIC
		-fexperimental-library
		-stdlib=libc++
		-lc++abi
	)
endif()

if(ENABLE_TESTING)
	find_package(Catch2 REQUIRED)
	enable_testing()
	add_executable(
		util-tests
		./tests/integers.cpp
		./tests/static-string.cpp
		./tests/variants.cpp
		./tests/pairwise-tuple.cpp
		./tests/bits.cpp
		./tests/meta.cpp
		./tests/bit-adaptors.cpp
		./tests/integer-variant.cpp
		./tests/golden-section-search.cpp
		./tests/arithmetic-friends.cpp)

	target_link_libraries(util-tests util Catch2::Catch2WithMain Catch2::Catch2)

	include(CTest)
	include(Catch)
	catch_discover_tests(util-tests TEST_PREFIX "util-")
endif()

if(ENABLE_PCH)
	target_precompile_headers(
		project_options
		INTERFACE
		./include/util/io.hpp
		./include/util/misc.hpp
		./include/util/container-operations.hpp
		./include/util/views.hpp
		./include/util/concepts.hpp
		./include/util/cv-adaptors.hpp
		./include/util/static-vector.hpp)
endif()

set_property(TARGET util PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET util PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET util
	APPEND PROPERTY
	COMPATIBLE_INTERFACE_STRING ${PROJECT_VERSION_MAJOR})

install(TARGETS util EXPORT ug-util-targets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
	FILE_SET HEADERS)

install(EXPORT ug-util-targets
	FILE ug-util-targets.cmake
	NAMESPACE ug::
	DESTINATION lib/cmake/ug/util)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/ug-util-config-version.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion)

configure_package_config_file(
	cmake/config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/ug-util-config.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/ug/util/cmake"
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/ug-util-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/ug-util-config-version.cmake"
	DESTINATION lib/cmake/ug/util)
