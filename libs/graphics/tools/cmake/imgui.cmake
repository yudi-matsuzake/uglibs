set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

set(IMGUI_SRC
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/examples/imgui_impl_opengl3.cpp
	${IMGUI_DIR}/examples/imgui_impl_glfw.cpp
	${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)

set(IMGUI_COMPILE_DEFINITIONS -DIMGUI_IMPL_OPENGL_LOADER_GLAD)

set(IMGUI_INCLUDE ${IMGUI_DIR} ${IMGUI_DIR}/examples)

add_library(ug_imgui STATIC "${IMGUI_SRC}")
target_link_libraries(ug_imgui PUBLIC ug_glad)
target_compile_definitions(ug_imgui PUBLIC ${IMGUI_COMPILE_DEFINITIONS})
target_include_directories(ug_imgui
	SYSTEM PUBLIC
		$<BUILD_INTERFACE:${IMGUI_DIR}>
		$<BUILD_INTERFACE:${IMGUI_DIR}/examples>
		$<INSTALL_INTERFACE:include/ug/imgui>
		$<INSTALL_INTERFACE:include/ug/examples>
)
