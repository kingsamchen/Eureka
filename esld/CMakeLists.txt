cmake_minimum_required(VERSION 3.19)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(ESLD_NOT_SUBPROJECT ON)
else()
  set(ESLD_NOT_SUBPROJECT OFF)
endif()

message(STATUS "ESLD as root project = ${ESLD_NOT_SUBPROJECT}")

project(ESLD CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(ESLD_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(ESLD_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ESLD_CMAKE_DIR ${ESLD_DIR}/cmake)

include(CTest)
include(${ESLD_CMAKE_DIR}/CPM.cmake)

message(STATUS "ESLD GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(ESLD_NOT_SUBPROJECT)
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
    message(STATUS "ESLD BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "ESLD BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${ESLD_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${ESLD_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${ESLD_CMAKE_DIR}/clang_tidy.cmake)

add_subdirectory(esld)

if(ESLD_NOT_SUBPROJECT AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
