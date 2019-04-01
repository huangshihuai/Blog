cmake_minimum_required (VERSION 2.8.11)
project(memory_model)

set(LRU_CFLAGS "-O0 -g3 -std=c++11")

set(CMAKE_C_FLAGS "${LRU_CFLAGS}")
set(CMAKE_CXX_FLAGS "${LRU_CFLAGS}")

add_executable(model model.cpp)
