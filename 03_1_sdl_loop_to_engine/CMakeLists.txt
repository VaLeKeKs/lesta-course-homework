cmake_minimum_required(VERSION 3.25)
project(03_1_sdl_loop_to_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(03_1_sdl_loop_to_engine src/main.cpp src/engine.cxx src/engine.hxx)

find_package(SDL3 REQUIRED)

target_link_libraries(03_1_sdl_loop_to_engine PRIVATE SDL3::SDL3-shared)
