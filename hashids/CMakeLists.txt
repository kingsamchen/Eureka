cmake_minimum_required(VERSION 3.19)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(HASHIDS_NOT_SUBPROJECT ON)
else()
  set(HASHIDS_NOT_SUBPROJECT OFF)
endif()

message(STATUS "hashids as root project = ${HASHIDS_NOT_SUBPROJECT}")

project(hashids CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(HASHIDS_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(HASHIDS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HASHIDS_CMAKE_DIR ${HASHIDS_DIR}/cmake)

include(CTest)
include(${HASHIDS_CMAKE_DIR}/CPM.cmake)

message(STATUS "hashids GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(HASHIDS_NOT_SUBPROJECT)
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
    message(STATUS "hashids BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "hashids BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${HASHIDS_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${HASHIDS_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${HASHIDS_CMAKE_DIR}/clang_tidy.cmake)

CPMAddPackage("gh:kingsamchen/esl#80f7d2107d7d92952abeae6cfada4359140831f9")
CPMAddPackage("gh:onqtam/doctest#v2.4.11")
CPMAddPackage("gh:fmtlib/fmt#10.0.0")
CPMAddPackage("gh:p-ranav/argparse#v2.9")

add_subdirectory(hashids)
add_subdirectory(cmd)

if(HASHIDS_NOT_SUBPROJECT AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
