project(heap)
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_VERBOSE_MAKEFILE on)

set(HEAP_CFLAGS "-O0 -g3 -std=c++11")

set(CMAKE_C_FLAGS "${HEAP_CFLAGS}")
set(CMAKE_CXX_FLAGS "${HEAP_CFLAGS}")

add_executable(assign_cookies assign_cookies.cpp)
add_executable(candy candy.cpp)
add_executable(overlapping Non-overlappingIntervals.cpp)
