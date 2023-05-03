cmake_minimum_required(VERSION 3.19)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(HTTP_MULTIPART_NOT_SUBPROJECT ON)
else()
  set(HTTP_MULTIPART_NOT_SUBPROJECT OFF)
endif()

message(STATUS "http-multipart as root project = ${HTTP_MULTIPART_NOT_SUBPROJECT}")

project(http-multipart CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(HTTP_MULTIPART_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(HTTP_MULTIPART_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HTTP_MULTIPART_CMAKE_DIR ${HTTP_MULTIPART_DIR}/cmake)

include(CTest)
include(${HTTP_MULTIPART_CMAKE_DIR}/CPM.cmake)

message(STATUS "http-multipart GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(HTTP_MULTIPART_NOT_SUBPROJECT)
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
    message(STATUS "http-multipart BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "http-multipart BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${HTTP_MULTIPART_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${HTTP_MULTIPART_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${HTTP_MULTIPART_CMAKE_DIR}/clang_tidy.cmake)

find_package(doctest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
CPMAddPackage("gh:kingsamchen/esl#caeca99503f5566782d7f60b44582c01761b3117")

add_subdirectory(multipart)
