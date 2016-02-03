cmake_minimum_required (VERSION 2.8)

project(ActiveThread)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-std=c++1y -pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../bin/")
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/../src/*.cpp")
add_executable(active-thread-demo ${SOURCE_FILES})