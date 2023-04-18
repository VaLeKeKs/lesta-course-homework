cmake_minimum_required(VERSION 3.22)
project(03_3_sdl_loop_hot_reload)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(03_3_sdl_loop_hot_reload src/engine.cxx)

set_target_properties(03_3_sdl_loop_hot_reload PROPERTIES ENABLE_EXPORTS 1)

find_package(SDL3 REQUIRED)
target_link_libraries(03_3_sdl_loop_hot_reload PRIVATE SDL3::SDL3-shared)

add_library(game SHARED src/game.cxx)
target_link_libraries(game PRIVATE 03_3_sdl_loop_hot_reload)


