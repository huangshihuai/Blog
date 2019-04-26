project(dlmain)
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_VERSION_MAKEFILE on)

set(DLMAIN_CFLAGS "-g0 -O3 -fPIC -ldl")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS "${DLMAIN_CFLAGS}")
set(CMAKE_CXX_FLAGS "${DLMAIN_CFLAGS}")
add_executable(dlmain dlmain.cpp)

add_subdirectory("hookmain")

