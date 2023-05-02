cmake_minimum_required(VERSION 3.25)
project(04_02_triangle_render)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(04_02_triangle_render src/triangle_main.cpp include/triangle_render.hxx include/canvas.hxx include/line_render.hxx include/triangle_interpolated.hxx include/triangle_indexed.hxx include/gfx.hxx src/gfx_render.cxx)

add_executable(gfx src/gfx_render.cxx)

find_package(SDL3 REQUIRED)

target_link_libraries(gfx PRIVATE SDL3::SDL3-static)

