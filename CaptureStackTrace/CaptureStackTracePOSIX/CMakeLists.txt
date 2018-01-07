cmake_minimum_required(VERSION 2.8)

project(CaptureStackTrace)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../")
file(GLOB SOURCES "src/main.cpp")

add_executable(capture_stack_trace ${SOURCES})
