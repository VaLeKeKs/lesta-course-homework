cmake_minimum_required(VERSION 3.22)
project(hello_world_lib CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(hello_world_static STATIC src/hello_world.cpp)
target_include_directories(hello_world_static PUBLIC include)

add_library(hello_world_dynamic SHARED src/hello_world.cpp)
target_include_directories(hello_world_dynamic PUBLIC include)
