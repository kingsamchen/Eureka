cmake_minimum_required(VERSION 3.21)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(JSON_SERDE_NOT_SUBPROJECT ON)
else()
  set(JSON_SERDE_NOT_SUBPROJECT OFF)
endif()

message(STATUS "json-serde as root project = ${JSON_SERDE_NOT_SUBPROJECT}")

project(json-serde CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(JSON_SERDE_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(JSON_SERDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(JSON_SERDE_CMAKE_DIR ${JSON_SERDE_DIR}/cmake)

include(CTest)

message(STATUS "json-serde GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(JSON_SERDE_NOT_SUBPROJECT)
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
    message(STATUS "json-serde BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "json-serde BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${JSON_SERDE_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${JSON_SERDE_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${JSON_SERDE_CMAKE_DIR}/clang_tidy.cmake)

find_package(nlohmann_json CONFIG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec")

add_subdirectory(serde)
