cmake_minimum_required(VERSION 3.11)

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(AUTO-CFS-CORES_NOT_SUBPROJECT ON)
endif()

project(auto-cfs-cores CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(AUTO-CFS-CORES_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
  set(DEPS_SOURCE_DIR ${ROOT_DIR}/build/deps)
  set(DEPS_DEPLOY_DIR ${CMAKE_BINARY_DIR})
endif()

# Add options here.

set(AUTO-CFS-CORES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(AUTO-CFS-CORES_DEPS_DIR ${AUTO-CFS-CORES_DIR}/deps)
set(AUTO-CFS-CORES_CMAKE_DIR ${AUTO-CFS-CORES_DIR}/cmake)

include(${AUTO-CFS-CORES_CMAKE_DIR}/dependency_manager.cmake)

# Compiler and output configurations.
include(${AUTO-CFS-CORES_CMAKE_DIR}/compiler_posix.cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(auto_cfs_cores)
add_subdirectory(tests)
