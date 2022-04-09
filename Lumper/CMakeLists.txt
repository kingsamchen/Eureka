cmake_minimum_required(VERSION 3.19)

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(LUMPER_NOT_SUBPROJECT ON)
else()
  set(LUMPER_NOT_SUBPROJECT OFF)
endif()

message(STATUS "Lumper as root project = ${LUMPER_NOT_SUBPROJECT}")

project(Lumper CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(LUMPER_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(LUMPER_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LUMPER_CMAKE_DIR ${LUMPER_DIR}/cmake)

include(CTest)
include(${LUMPER_CMAKE_DIR}/CPM.cmake)

message(STATUS "Lumper GENERATOR = " ${CMAKE_GENERATOR})

set(LUMPER_PCH_HEADER ${LUMPER_DIR}/out/pch/precompile.h)

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(LUMPER_NOT_SUBPROJECT)
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
    message(STATUS "Lumper BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "Lumper BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
include(${LUMPER_CMAKE_DIR}/compiler_posix.cmake)

include(${LUMPER_CMAKE_DIR}/clang_tidy.cmake)

add_subdirectory(base)
add_subdirectory(lumper)

if(LUMPER_NOT_SUBPROJECT AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
