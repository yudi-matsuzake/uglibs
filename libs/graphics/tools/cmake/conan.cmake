macro(run_conan)
  # Download automatically, you can also just copy the conan.cmake file
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
  endif()

  set(ENV{CONAN_REVISIONS_ENABLED} 1)
  include(${CMAKE_BINARY_DIR}/conan.cmake)

  conan_cmake_configure(
    REQUIRES
    glm/0.9.9.8
    glad/0.1.36
    imgui/cci.20220621+1.88.docking
    GENERATORS cmake_find_package
    OPTIONS BASIC_SETUP CMAKE_TARGETS
    BUILD missing
    OPTIONS glad:spec=gl glad:gl_version=4.6 glad:gl_profile=core
    # IMPORTS "imgui/res/bindings, imgui_impl_opengl3.* -> ./bindings"
    # IMPORTS "imgui/res/bindings, imgui_impl_glfw.* -> ./bindings"
    IMPORTS "./res/bindings, imgui_impl_glfw.cpp -> ${CMAKE_BINARY_DIR}/imgui-bindings"
    IMPORTS "./res/bindings, imgui_impl_opengl3.cpp -> ${CMAKE_BINARY_DIR}/imgui-bindings"
    IMPORTS "./res/bindings, imgui_impl_glfw.h -> ${CMAKE_BINARY_DIR}/imgui-bindings"
    IMPORTS "./res/bindings, imgui_impl_opengl3.h -> ${CMAKE_BINARY_DIR}/imgui-bindings"
    IMPORTS "./res/bindings, imgui_impl_opengl3_loader.h -> ${CMAKE_BINARY_DIR}/imgui-bindings"
)

  conan_cmake_autodetect(settings)

  conan_cmake_install(
    PATH_OR_REFERENCE .
    BUILD missing
    SETTINGS ${settings})
endmacro()
