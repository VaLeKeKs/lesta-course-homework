cmake_minimum_required(VERSION 3.22)
project(04_00_canvas)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(04_00_canvas src/canvas_main.cpp include/canvas.hxx)
