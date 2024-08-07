cmake_minimum_required(VERSION 3.21)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(CPR_STRUCT_PARAMETERS_NOT_SUBPROJECT ON)
else()
  set(CPR_STRUCT_PARAMETERS_NOT_SUBPROJECT OFF)
endif()

message(STATUS "cpr-struct-parameters as root project = ${CPR_STRUCT_PARAMETERS_NOT_SUBPROJECT}")

project(cpr-struct-parameters CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CPR_STRUCT_PARAMETERS_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(CPR_STRUCT_PARAMETERS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CPR_STRUCT_PARAMETERS_CMAKE_DIR ${CPR_STRUCT_PARAMETERS_DIR}/cmake)

include(CTest)

message(STATUS "cpr-struct-parameters GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(CPR_STRUCT_PARAMETERS_NOT_SUBPROJECT)
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
    message(STATUS "cpr-struct-parameters BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "cpr-struct-parameters BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${CPR_STRUCT_PARAMETERS_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${CPR_STRUCT_PARAMETERS_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${CPR_STRUCT_PARAMETERS_CMAKE_DIR}/clang_tidy.cmake)
include(${CPR_STRUCT_PARAMETERS_CMAKE_DIR}/sanitizer.cmake)

find_package(cpr CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)

add_subdirectory(main)
