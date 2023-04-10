cmake_minimum_required(VERSION 3.22)
project(hello_world_2)


if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()


add_subdirectory(hello_bin)
add_subdirectory(hello_lib)
