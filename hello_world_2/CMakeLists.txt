cmake_minimum_required(VERSION 3.22)
set(CMAKE_GENERATOR Ninja)
project(hello_world_2)

add_subdirectory(hello_bin)
add_subdirectory(hello_lib)

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

