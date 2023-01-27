cmake_minimum_required(VERSION 3.0)

project(CppPython)

set(SRC_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-std=c++23 -g -Wall")

set(CMAKE_BUILD_TYPE "Debug")

set(EXECUTABLE_OUTPUT_PATH ${SRC_ROOT_PATH})

file(GLOB OBJECTS "${SRC_ROOT_PATH}/src/objects/*.cpp")
file(GLOB READER "${SRC_ROOT_PATH}/src/reader/*.cpp")
file(GLOB RUNTIME "${SRC_ROOT_PATH}/src/runtime/*.cpp")

message(${OBJECTS})
message(${READER})
message(${RUNTIME})

set(
    RESOURCE_FILES
    ${OBJECTS}    
    ${READER}    
    ${RUNTIME}    
)

add_executable(CppPython ${RESOURCE_FILES} "CppPython.cpp")

add_executable(SimpleTest ${RESOURCE_FILES} "SimpleTest.cpp")

add_executable(a "a.cpp")
