# When IMGUI_DIR is set, copies imgui backends into
# ${CMAKE_BINARY_DIR}/imgui-bindings/ for the ugimgui target.
# No-op when IMGUI_DIR is unset.

if(DEFINED ENV{IMGUI_DIR})
    set(_imgui_dir $ENV{IMGUI_DIR})
    if(NOT IS_DIRECTORY "${_imgui_dir}")
        message(FATAL_ERROR "nix-toolchain: IMGUI_DIR=${_imgui_dir} is not a directory")
    endif()

    set(_bindings_out "${CMAKE_BINARY_DIR}/imgui-bindings")
    file(MAKE_DIRECTORY "${_bindings_out}")

    file(COPY "${_imgui_dir}/backends/imgui_impl_glfw.cpp" DESTINATION "${_bindings_out}")
    file(COPY "${_imgui_dir}/backends/imgui_impl_glfw.h" DESTINATION "${_bindings_out}")
    file(COPY "${_imgui_dir}/backends/imgui_impl_opengl3.cpp" DESTINATION "${_bindings_out}")
    file(COPY "${_imgui_dir}/backends/imgui_impl_opengl3.h" DESTINATION "${_bindings_out}")
    file(COPY "${_imgui_dir}/backends/imgui_impl_opengl3_loader.h" DESTINATION "${_bindings_out}")
    file(COPY "${_imgui_dir}/misc/cpp/imgui_stdlib.cpp" DESTINATION "${_bindings_out}")
    file(COPY "${_imgui_dir}/misc/cpp/imgui_stdlib.h" DESTINATION "${_bindings_out}")

    set(IMGUI_DIR "${_imgui_dir}")
    message(STATUS "nix-toolchain: imgui bindings copied to ${_bindings_out}")
else()
    message(STATUS "nix-toolchain: IMGUI_DIR not set, skipping")
endif()
