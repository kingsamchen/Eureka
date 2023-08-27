cmake_minimum_required(VERSION 3.20)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(TRY_VCPKG_NOT_SUBPROJECT ON)
else()
  set(TRY_VCPKG_NOT_SUBPROJECT OFF)
endif()

message(STATUS "try-vcpkg as root project = ${TRY_VCPKG_NOT_SUBPROJECT}")

project(try-vcpkg CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(TRY_VCPKG_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(TRY_VCPKG_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TRY_VCPKG_CMAKE_DIR ${TRY_VCPKG_DIR}/cmake)

include(CTest)

message(STATUS "try-vcpkg GENERATOR = " ${CMAKE_GENERATOR})

set(TRY_VCPKG_PCH_HEADER ${TRY_VCPKG_DIR}/build/pch/precompile.h)

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(TRY_VCPKG_NOT_SUBPROJECT)
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
    message(STATUS "try-vcpkg BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "try-vcpkg BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${TRY_VCPKG_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${TRY_VCPKG_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${TRY_VCPKG_CMAKE_DIR}/clang_tidy.cmake)

find_package(fmt REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(esl REQUIRED)
find_package(uuidxx REQUIRED)

add_subdirectory(foobar)
