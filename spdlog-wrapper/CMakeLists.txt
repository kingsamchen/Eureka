cmake_minimum_required(VERSION 3.28)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(SPDLOG_WRAPPER_NOT_SUBPROJECT ON)
else()
  set(SPDLOG_WRAPPER_NOT_SUBPROJECT OFF)
endif()

message(STATUS "spdlog-wrapper as root project = ${SPDLOG_WRAPPER_NOT_SUBPROJECT}")

project(spdlog-wrapper CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(SPDLOG_WRAPPER_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(SPDLOG_WRAPPER_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SPDLOG_WRAPPER_CMAKE_DIR ${SPDLOG_WRAPPER_DIR}/cmake)

include(CTest)

message(STATUS "spdlog-wrapper GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(SPDLOG_WRAPPER_NOT_SUBPROJECT)
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
    message(STATUS "spdlog-wrapper BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "spdlog-wrapper BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${SPDLOG_WRAPPER_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${SPDLOG_WRAPPER_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${SPDLOG_WRAPPER_CMAKE_DIR}/clang_tidy.cmake)
include(${SPDLOG_WRAPPER_CMAKE_DIR}/sanitizer.cmake)

set(GFLAGS_USE_TARGET_NAMESPACE ON)
find_package(gflags CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(main)
