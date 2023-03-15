cmake_minimum_required(VERSION 3.0)

project(CppPython)

set(PROJECT_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "-std=c++23 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE  "-std=c++23 -O3 ")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_ROOT_PATH}/bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_ROOT_PATH}/bin/release")


file(GLOB OBJECTS "${PROJECT_ROOT_PATH}/src/objects/*.cpp")
file(GLOB READER "${PROJECT_ROOT_PATH}/src/reader/*.cpp")
file(GLOB RUNTIME "${PROJECT_ROOT_PATH}/src/runtime/*.cpp")

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

add_executable(show_hex "show_hex.cpp")

add_executable(a "a.cpp")
