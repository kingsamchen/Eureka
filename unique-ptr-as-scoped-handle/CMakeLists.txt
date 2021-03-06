cmake_minimum_required(VERSION 3.16)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(SCOPED_HANDLE_NOT_SUBPROJECT ON)
endif()

project(scoped-handle CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(SCOPED_HANDLE_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})

  set_directory_properties(PROPERTIES VS_STARTUP_PROJECT "scopedhandle")
endif()

# Add options below.

set(SCOPED_HANDLE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SCOPED_HANDLE_CMAKE_DIR ${SCOPED_HANDLE_DIR}/cmake)

include(CTest)
include(${SCOPED_HANDLE_CMAKE_DIR}/CPM.cmake)

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
  string (REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  include(${SCOPED_HANDLE_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${SCOPED_HANDLE_CMAKE_DIR}/compiler_posix.cmake)
endif()

add_subdirectory(scopedhandle)

if (SCOPED_HANDLE_NOT_SUBPROJECT AND BUILD_TESTING)
  add_subdirectory(test)
endif()
