cmake_minimum_required(VERSION 2.8.11)
project(sort)

set(BUBLLE_CFLAGS "-O0 -g3 -std=c++11")

set(CMAKE_C_FLAGS "${BUBLLE_CFLAGS}")
set(CMAKE_CXX_FLAGS "${BUBLLE_CFLAGS}")

add_executable(sort main.cpp sort.h)
