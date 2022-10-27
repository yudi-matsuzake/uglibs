cmake_minimum_required(VERSION 3.14)

project(graphics
	VERSION 0.0.0
	DESCRIPTION "ug-graphics is a graphics library comprised of window, input, and graphics abstraction/facilities"
	LANGUAGES C CXX
)

# export information about compilation
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# including extra scripts and subdirectories
include(./tools/cmake/warnings.cmake)
include(./tools/cmake/glad.cmake)
include(./tools/cmake/imgui.cmake)

# set(GLM_TEST_ENABLE FALSE)
# set(GLM_QUIET ON)
# set(GLM_TEST_ENABLE_CXX_17 ON)
# add_subdirectory(./external/glm)

# debug flags
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS_DEBUG "-g3")
endif()

# find_package(Catch2 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# ug graphics library
add_library(
	graphics STATIC

	# graphics
	./src/opengl-error.cpp
	./src/component.cpp
	./src/misc.cpp
	./src/ball2d-render.cpp
	./src/shader.cpp
	./src/grid2d-render.cpp
	./src/vao.cpp
	./src/program.cpp
	./src/window.cpp
	./src/app.cpp
	./src/app2d.cpp
	./src/app3d.cpp
	./src/component-manager.cpp
	./src/buffers.cpp
	./src/render.cpp
	./src/basic-texture.cpp
	./src/texture1d.cpp
	./src/texture2d.cpp
	./src/camera.cpp
	./src/camera2d-controller.cpp
	./src/segment2d-render.cpp
	./src/scene.cpp
	./src/mesh3d-render.cpp
	./src/mesh3d.cpp

	./include/ug/graphics/program.hpp
	./include/ug/graphics/misc.hpp
	./include/ug/graphics/shader.hpp
	./include/ug/graphics/vao.hpp
	./include/ug/graphics/opengl-error.hpp
	./include/ug/graphics/grid2d-render.hpp
	./include/ug/graphics/component-manager.hpp
	./include/ug/graphics/component.hpp
	./include/ug/graphics/buffers.hpp
	./include/ug/graphics/window.hpp
	./include/ug/graphics/app.hpp
	./include/ug/graphics/app2d.hpp
	./include/ug/graphics/app3d.hpp
	./include/ug/graphics/ball2d-render.hpp
	./include/ug/graphics/render.hpp
	./include/ug/graphics/basic-texture.hpp
	./include/ug/graphics/texture1d.hpp
	./include/ug/graphics/texture2d.hpp
	./include/ug/graphics/camera.hpp
	./include/ug/graphics/camera2d-controller.hpp
	./include/ug/graphics/segment2d-render.hpp
	./include/ug/graphics/scene.hpp
	./include/ug/graphics/mesh3d.hpp
	./include/ug/graphics/mesh3d-render.hpp
)
target_link_libraries(
	graphics
	PUBLIC
		ug-graphics-warnings
		glfw
		OpenGL::GL
		imgui
		glad
		${OpenCV_LIBS}
		stdc++fs
		glm
)
target_link_directories(graphics PUBLIC $<INSTALL_INTERFACE:lib/ug>)
target_include_directories(
	graphics
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		${OpenCV_INCLUDE_DIRS}
)
target_include_directories(
	graphics
	SYSTEM PUBLIC
		$<BUILD_INTERFACE:${IMGUI_DIR}>
		$<INSTALL_INTERFACE:include>
							 
		$<BUILD_INTERFACE:${IMGUI_DIR}/examples>
		$<INSTALL_INTERFACE:include>

		# $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/glm>
)
target_compile_features(graphics PUBLIC cxx_std_20)

# test programs
option(GRAPHICS_TEST "Add test programs for graphics library" ON)

if(${GRAPHICS_TEST})
	add_executable(hello-graphics ./test/hello-graphics.cpp)
	target_link_libraries(hello-graphics PRIVATE graphics)

	add_executable(hello-segment ./test/hello-segment.cpp)
	target_link_libraries(hello-segment PRIVATE graphics)

	add_executable(hello-scene ./test/hello-scene.cpp)
	target_link_libraries(hello-scene PRIVATE graphics)
endif()

# installing ------------------------------------------------------------------

# installing headers
install(DIRECTORY ./include/ug DESTINATION include)
install(FILES ${IMGUI_HEADER} DESTINATION include/ug/imgui)

# FIXME: this will install glad.c instead of installing only the header files
install(DIRECTORY ${GLAD_DIR} DESTINATION include/ug)

# installation of libraries
install(TARGETS imgui glad graphics ug-graphics-warnings EXPORT ugConfig
	ARCHIVE  DESTINATION lib/ug
	LIBRARY  DESTINATION lib/ug
	RUNTIME  DESTINATION bin/ug
)

install(EXPORT ugConfig NAMESPACE ug:: DESTINATION share/ug/cmake)
# This makes the project importable from the build directory
export(TARGETS glad imgui graphics ug-graphics-warnings FILE ugConfig.cmake)
