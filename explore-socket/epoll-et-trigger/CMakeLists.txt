cmake_minimum_required(VERSION 3.14)

# TODO: Add POLICY here.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(EPET_NOT_SUBPROJECT ON)
endif()

project(EPET CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(EPET_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

add_executable(epet epet_server.cpp)
