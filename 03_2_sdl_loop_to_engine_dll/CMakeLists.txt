cmake_minimum_required(VERSION 3.22)
project(03_2_sdl_loop_to_engine_dll)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(game_03_2 src/main.cpp)
target_link_libraries(game_03_2 PRIVATE engine_03_2)

find_package(SDL3 REQUIRED)

add_library(engine_03_2 SHARED src/engine.cxx src/engine.hxx)
target_link_libraries(engine_03_2 PRIVATE SDL3::SDL3-shared)