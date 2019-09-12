cmake_minimum_required(VERSION 3.14)
project(graphic-tests)

find_package(ug REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(hello-graphics main.cpp)
target_link_libraries(hello-graphics ug::graphics)
