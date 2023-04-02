cmake_minimum_required(VERSION 3.22)
project(hello_world_bin CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello_bin_static src/main.cpp)
target_link_libraries(hello_bin_static PUBLIC hello_world_static)

add_executable(hello_bin_dynamic src/main.cpp)
target_link_libraries(hello_bin_dynamic PUBLIC hello_world_dynamic)
