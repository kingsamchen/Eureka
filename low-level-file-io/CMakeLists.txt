cmake_minimum_required(VERSION 3.19)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(LOW_LEVEL_FILE_IO_NOT_SUBPROJECT ON)
else()
  set(LOW_LEVEL_FILE_IO_NOT_SUBPROJECT OFF)
endif()

message(STATUS "low-level-file-io as root project = ${LOW_LEVEL_FILE_IO_NOT_SUBPROJECT}")

project(low-level-file-io CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(LOW_LEVEL_FILE_IO_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(LOW_LEVEL_FILE_IO_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LOW_LEVEL_FILE_IO_CMAKE_DIR ${LOW_LEVEL_FILE_IO_DIR}/cmake)

include(CTest)
include(${LOW_LEVEL_FILE_IO_CMAKE_DIR}/CPM.cmake)

message(STATUS "low-level-file-io GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(LOW_LEVEL_FILE_IO_NOT_SUBPROJECT)
  if(MULTICONF_GENERATOR)
    foreach(OUTPUTCONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
      string(TOUPPER ${OUTPUTCONFIG_TYPE} OUTPUTCONFIG)
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG_TYPE}/bin)
      set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG_TYPE}/lib)
    endforeach()
  else()
    if(NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE "Release")
    endif()
    message(STATUS "low-level-file-io BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "low-level-file-io BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
include(${LOW_LEVEL_FILE_IO_CMAKE_DIR}/compiler_posix.cmake)

include(${LOW_LEVEL_FILE_IO_CMAKE_DIR}/clang_tidy.cmake)

add_subdirectory(file_io)

