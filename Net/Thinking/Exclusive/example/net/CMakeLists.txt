
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE NET "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


set(LIBRARY_OUTPUT_PATH ${NETOUTPUT}/lib)
add_library(net-shared SHARED ${NET})
add_library(net-static STATIC ${NET})