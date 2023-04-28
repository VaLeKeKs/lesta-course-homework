cmake_minimum_required(VERSION 3.22)
project(04_01_line_render)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(04_01_line_render src/line_render_main.cxx include/line_render.hxx ../04_00_canvas/include/canvas.hxx)

