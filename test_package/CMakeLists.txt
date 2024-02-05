cmake_minimum_required(VERSION 3.23)
project(PackageTest CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

find_package(uglibs REQUIRED VERSION 0.0.1 COMPONENTS util gmt graphics ia containers)

add_executable(example src/example.cpp)
target_compile_features(example INTERFACE cxx_std_20)
target_link_libraries(example util gmt graphics ia containers)
