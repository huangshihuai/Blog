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

set(NETOUTPUT ${CMAKE_BINARY_DIR}/output)
link_directories(${NETOUTPUT}/lib)

add_subdirectory(net)

#  define macro 
macro(add_example_target EXAMPLE_PATH EXAMPLE_TARGET)
    add_executable("example_${EXAMPLE_TARGET}" "${EXAMPLE_PATH}/example_${EXAMPLE_TARGET}.cpp")
    target_link_libraries("example_${EXAMPLE_TARGET}" net-static)
    target_link_libraries("example_${EXAMPLE_TARGET}" pthread) 
endmacro(add_example_target)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${NETOUTPUT}/bin)
add_example_target("." client)
add_example_target(${CMAKE_CURRENT_SOURCE_DIR}/no_support lt)