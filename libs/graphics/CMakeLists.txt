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
find_package(glfw3 3.3 REQUIRED)
# find_package(glm REQUIRED)

# ug graphics library
add_library(
	graphics STATIC

	# graphics
	./src/opengl-error.cpp
	./src/component.cpp
	./src/misc.cpp
	./src/ball2d-render.cpp
	./src/square-render.cpp
	./src/shader.cpp
	./src/grid2d-render.cpp
	./src/vao.cpp
	./src/program.cpp
	./src/app.cpp
	./src/component-manager.cpp
	./src/buffers.cpp
	./src/orthographic-zoom.cpp
	./src/render.cpp
	./src/translation2d.cpp
	./src/texture2d.cpp

	./include/ug/graphics/program.hpp
	./include/ug/graphics/misc.hpp
	./include/ug/graphics/shader.hpp
	./include/ug/graphics/vao.hpp
	./include/ug/graphics/opengl-error.hpp
	./include/ug/graphics/grid2d-render.hpp
	./include/ug/graphics/component-manager.hpp
	./include/ug/graphics/component.hpp
	./include/ug/graphics/square-render.hpp
	./include/ug/graphics/buffers.hpp
	./include/ug/graphics/app.hpp
	./include/ug/graphics/ball2d-render.hpp
	./include/ug/graphics/orthographic-zoom.hpp
	./include/ug/graphics/render.hpp
	./include/ug/graphics/translation2d.hpp
	./include/ug/graphics/texture2d.hpp

)
target_link_libraries(
	graphics
	PUBLIC ug-graphics-warnings glfw OpenGL::GL imgui glad ${OpenCV_LIBS}
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
target_compile_features(graphics PUBLIC cxx_std_17)

# include(CTest)
# include(Catch)
# catch_discover_tests(shrep-tests)

# installing ------------------------------------------------------------------

# installing headers
install(DIRECTORY ./include/ug DESTINATION include)
install(DIRECTORY ${IMGUI_DIR} DESTINATION include/ug)
install(DIRECTORY ${GLAD_DIR} DESTINATION include/ug)

# installation of libraries
install(TARGETS imgui glad graphics ug-graphics-warnings EXPORT ugConfig
	ARCHIVE  DESTINATION lib/ug
	LIBRARY  DESTINATION lib/ug
	RUNTIME  DESTINATION bin/ug
)

install(EXPORT ugConfig NAMESPACE ug:: DESTINATION share/ug/cmake)
export(TARGETS glad imgui graphics ug-graphics-warnings FILE ugConfig.cmake)
