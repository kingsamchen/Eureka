cmake_minimum_required(VERSION 3.1)

project(ConcurrentHttpServer CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fno-limit-debug-info")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()

include_directories("../../../../KBase/src")

set(PROJECT_LINK_LIBS "libkbase.a" "pthread")
link_directories(${CMAKE_BINARY_DIR}/../)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../")
set(SOURCES
    main.cpp
    tcp_connection_manager.cpp
    tcp_connection.cpp
    worker.cpp
    )

add_executable(khttpd ${SOURCES})
target_link_libraries(khttpd ${PROJECT_LINK_LIBS})

