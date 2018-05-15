
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE API "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


set(LIBRARY_OUTPUT_PATH ${APIOUTPUT}/lib)
add_library(api-shared SHARED ${API})
add_library(api-static STATIC ${API})