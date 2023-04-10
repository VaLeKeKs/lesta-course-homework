cmake_minimum_required(VERSION 3.22)
project(02_sdl_dynamic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(02_sdl_dynamic src/main.cpp)

find_package(SDL3 REQUIRED)

target_link_libraries(02_sdl_dynamic PRIVATE SDL3::SDL3-shared)

include(CTest)
add_test(NAME pytest COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh)