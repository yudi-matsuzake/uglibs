cmake_minimum_required(VERSION 3.16)
project(graphics
	DESCRIPTION "ug-graphics is a graphics library comprised of window, input, and graphics abstraction/facilities"
	VERSION 0.0.0
	LANGUAGES C CXX)


# export information about compilation
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)


# debug flags
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS_DEBUG "-g3")
endif()

add_library(ug_graphics_project_warnings INTERFACE)

# including extra scripts and subdirectories
include(./tools/cmake/warnings.cmake)

# find_package(Catch2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED VERSION 0.9.9.8)
find_package(glad REQUIRED VERSION 0.1.36)
find_package(Boost REQUIRED COMPONENTS gil)

find_package(imgui REQUIRED VERSION 0.1.36)

add_library(ugimgui
	"${CMAKE_BINARY_DIR}/imgui-bindings/imgui_impl_glfw.h"
	"${CMAKE_BINARY_DIR}/imgui-bindings/imgui_impl_opengl3_loader.h"
	"${CMAKE_BINARY_DIR}/imgui-bindings/imgui_impl_opengl3.h"

	"${CMAKE_BINARY_DIR}/imgui-bindings/imgui_impl_glfw.cpp"
	"${CMAKE_BINARY_DIR}/imgui-bindings/imgui_impl_opengl3.cpp")

target_include_directories(
	ugimgui PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/imgui-bindings>"
	"$<INSTALL_INTERFACE:include/imgui-bindings>")

target_link_libraries(ugimgui PUBLIC imgui::imgui glfw)

add_library(graphics STATIC

	include/ug/graphics/inputs.hpp
	include/ug/graphics/camera3d-controller.hpp
	include/ug/graphics/camera2d-controller.hpp
	include/ug/graphics/texture2d.hpp
	include/ug/graphics/misc.hpp
	include/ug/graphics/light3d.hpp
	include/ug/graphics/app.hpp
	include/ug/graphics/buffer-texture.hpp
	include/ug/graphics/texture1d.hpp
	include/ug/graphics/ball2d-render.hpp
	include/ug/graphics/basic-texture.hpp
	include/ug/graphics/camera.hpp
	include/ug/graphics/buffers.hpp
	include/ug/graphics/app3d.hpp
	include/ug/graphics/component.hpp
	include/ug/graphics/app2d.hpp
	include/ug/graphics/segment2d-render.hpp
	include/ug/graphics/grid2d-render.hpp
	include/ug/graphics/vao.hpp
	include/ug/graphics/window.hpp
	include/ug/graphics/program.hpp
	include/ug/graphics/mesh3d.hpp
	include/ug/graphics/render.hpp
	include/ug/graphics/opengl-error.hpp
	include/ug/graphics/component-manager.hpp
	include/ug/graphics/mesh3d-render.hpp
	include/ug/graphics/shader.hpp

	src/misc.cpp
	src/component.cpp
	src/segment2d-render.cpp
	src/camera2d-controller.cpp
	src/window.cpp
	src/vao.cpp
	src/shader.cpp
	src/program.cpp
	src/camera.cpp
	src/component-manager.cpp
	src/grid2d-render.cpp
	src/render.cpp
	src/buffers.cpp
	src/buffer-texture.cpp
	src/basic-texture.cpp
	src/app.cpp
	src/mesh3d-render.cpp
	src/texture1d.cpp
	src/texture2d.cpp
	src/app3d.cpp
	src/app2d.cpp
	src/mesh3d.cpp
	src/camera3d-controller.cpp
	src/opengl-error.cpp
	src/ball2d-render.cpp
)

add_library(ug::graphics ALIAS graphics)

target_include_directories(
	graphics PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_compile_features(graphics PUBLIC cxx_std_20)

target_link_libraries(
	graphics
	PUBLIC
		ug-graphics-warnings
		glfw
		OpenGL::GL
		glad::glad
		ugimgui
		Boost::boost
		stdc++fs
		glm::glm
)

target_compile_definitions(graphics PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_directories(graphics PUBLIC $<INSTALL_INTERFACE:lib/ug>)
target_include_directories(
	graphics
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

target_include_directories(
	graphics
	SYSTEM PUBLIC
		$<BUILD_INTERFACE:${IMGUI_DIR}>
		$<INSTALL_INTERFACE:include>
							 
		$<BUILD_INTERFACE:${IMGUI_DIR}/examples>
		$<INSTALL_INTERFACE:include>
)
if(ENABLE_PCH)
	target_precompile_headers(
		graphics
		INTERFACE
		include/ug/graphics/inputs.hpp
		include/ug/graphics/camera2d-controller.hpp
		include/ug/graphics/texture2d.hpp
		include/ug/graphics/misc.hpp
		include/ug/graphics/app.hpp
		include/ug/graphics/ball2d-render.hpp
		include/ug/graphics/camera.hpp
		include/ug/graphics/buffers.hpp
		include/ug/graphics/app3d.hpp
		include/ug/graphics/component.hpp
		include/ug/graphics/app2d.hpp
		include/ug/graphics/segment2d-render.hpp
		include/ug/graphics/grid2d-render.hpp
		include/ug/graphics/vao.hpp
		include/ug/graphics/window.hpp
		include/ug/graphics/program.hpp
		include/ug/graphics/scene.hpp
		include/ug/graphics/mesh3d.hpp
		include/ug/graphics/render.hpp
		include/ug/graphics/opengl-error.hpp
		include/ug/graphics/component-manager.hpp
		include/ug/graphics/mesh3d-render.hpp
		include/ug/graphics/shader.hpp
)
endif()

target_compile_features(graphics PUBLIC cxx_std_20)
target_compile_options(graphics PUBLIC -DGLM_ENABLE_EXPERIMENTAL)

# test programs
option(GRAPHICS_TEST "Add test programs for graphics library" ON)

if(${GRAPHICS_TEST})
	add_executable(hello-graphics ./test/hello-graphics.cpp)
	target_link_libraries(hello-graphics PRIVATE graphics)

	add_executable(hello-segment ./test/hello-segment.cpp)
	target_link_libraries(hello-segment PRIVATE graphics)

	add_executable(hello-cube ./test/hello-cube.cpp)
	target_link_libraries(hello-cube PRIVATE graphics)

	add_executable(raymarching-primitives ./test/raymarching-primitives.cpp)
	target_link_libraries(raymarching-primitives PRIVATE graphics)
endif()

set_property(TARGET graphics PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET graphics PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET graphics
	APPEND PROPERTY
	COMPATIBLE_INTERFACE_STRING ${PROJECT_VERSION_MAJOR})

install(TARGETS graphics ug-graphics-warnings ugimgui
	EXPORT ug-graphics-targets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include)

install(EXPORT ug-graphics-targets
	FILE ug-graphics-targets.cmake
	NAMESPACE ug::graphics::
	DESTINATION lib/cmake/ug/graphics)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/ug-graphics-config-version.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion)

configure_package_config_file(
	cmake/config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/ug-graphics-config.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/ug/graphics/cmake"
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/ug-graphics-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/ug-graphics-config-version.cmake"
	DESTINATION lib/cmake/ug/graphics)
