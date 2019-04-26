set(MAIN_CFLAGS "-g0 -O3 -rdynamic -fPIC -pie")
set(CMAKE_C_FLAGS "${MAIN_CFLAGS}")
set(CMAKE_CXX_FLAGS "${MAIN_CFLAGS}")
add_executable(main main.cpp)
