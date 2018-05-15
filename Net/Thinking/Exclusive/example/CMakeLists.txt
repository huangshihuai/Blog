project(example)
cmake_minimum_required(VERSION 3.0)

set(ONDEBUG true)
if(${ONDEBUG})
    set(CMAKE_C_FLAGS "-g3 -O0")
    set(CMAKE_CXX_FLAGS "-g3 -O0")
else()
    set(CMAKE_C_FLAGS "-O2")
    set(CMAKE_CXX_FLAGS "-O2")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/api/include)

set(APIOUTPUT ${CMAKE_BINARY_DIR}/output)
link_directories(${APIOUTPUT}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APIOUTPUT}/bin)

add_subdirectory(api)

#  define macro 
macro(add_example_target EXAMPLE_PATH EXAMPLE_TARGET)
    add_executable("example_${EXAMPLE_TARGET}" "${EXAMPLE_PATH}/example_${EXAMPLE_TARGET}.cpp")
    target_link_libraries("example_${EXAMPLE_TARGET}" api-static)
    target_link_libraries("example_${EXAMPLE_TARGET}" pthread) 
endmacro(add_example_target)

add_example_target(client client)
add_example_target(no_support lt)
