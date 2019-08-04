cmake_minimum_required(VERSION 3.12)

project(asio-demo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(asio_dep)

add_executable(asio_demo main.cpp)

target_link_libraries(asio_demo PRIVATE asio)
