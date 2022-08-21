cmake_minimum_required(VERSION 3.17)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(LEARN_TOPT_NOT_SUBPROJECT ON)
endif()

project(learn-topt CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(LEARN_TOPT_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(LEARN_TOPT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LEARN_TOPT_CMAKE_DIR ${LEARN_TOPT_DIR}/cmake)

include(CTest)
include(${LEARN_TOPT_CMAKE_DIR}/CPM.cmake)

message(STATUS "GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
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
  message(STATUS "BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

# Compiler configurations.
if(MSVC)
  include(${LEARN_TOPT_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${LEARN_TOPT_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${LEARN_TOPT_CMAKE_DIR}/clang_tidy.cmake)

add_subdirectory(topt)
