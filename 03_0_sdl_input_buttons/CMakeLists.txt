cmake_minimum_required(VERSION 3.25)
project(03_0_sdl_input_buttons)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(03_0_sdl_input_buttons src/main.cpp)

find_package(SDL3 REQUIRED)
target_link_libraries(03_0_sdl_input_buttons PRIVATE SDL3::SDL3-shared)
