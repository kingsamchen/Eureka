cmake_minimum_required(VERSION 3.12)

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(LEARN-ASIO_NOT_SUBPROJECT ON)
endif()

project(learn-asio CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(LEARN-ASIO_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
  set(DEPS_SOURCE_DIR ${ROOT_DIR}/build/deps)
  set(DEPS_DEPLOY_DIR ${CMAKE_BINARY_DIR})
endif()

# Add options here.

set(LEARN-ASIO_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LEARN-ASIO_DEPS_DIR ${LEARN-ASIO_DIR}/deps)
set(LEARN-ASIO_CMAKE_DIR ${LEARN-ASIO_DIR}/cmake)

include(${LEARN-ASIO_CMAKE_DIR}/dependency_manager.cmake)

include(${LEARN-ASIO_CMAKE_DIR}/cotire.cmake)
set(LEARN-ASIO_PCH_HEADER ${LEARN-ASIO_DIR}/build/pch/precompile.h)

# Compiler and output configurations.
if(MSVC)
  string (REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  include(${LEARN-ASIO_CMAKE_DIR}/compiler_msvc.cmake)
  foreach(OUTPUTCONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG_TYPE} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG_TYPE}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG_TYPE}/lib)
  endforeach()
else()
  include(${LEARN-ASIO_CMAKE_DIR}/compiler_posix.cmake)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

# asio-cmake wrapper
set(ASIO_CMAKE_ASIO_TAG asio-1-12-2)
declare_dep_module(asio-cmake
  VERSION         latest
  GIT_REPOSITORY  https://github.com/kingsamchen/asio-cmake.git
  GIT_TAG         origin/master
)

declare_dep_module(KBase
  VERSION         6197a2af0
  GIT_REPOSITORY  https://github.com/kingsamchen/KBase.git
  GIT_TAG         6197a2af073107e45395a9ad38afdaf5f6850b93
)

add_subdirectory(timer)
add_subdirectory(daytime)
add_subdirectory(echo)
if(UNIX)
  add_subdirectory(handling_signal)
endif()
add_subdirectory(socks4a)
