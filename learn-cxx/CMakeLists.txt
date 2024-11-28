cmake_minimum_required(VERSION 3.25)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(LEARN_CXX_NOT_SUBPROJECT ON)
else()
  set(LEARN_CXX_NOT_SUBPROJECT OFF)
endif()

message(STATUS "learn-cxx as root project = ${LEARN_CXX_NOT_SUBPROJECT}")

project(learn-cxx CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(LEARN_CXX_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(LEARN_CXX_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LEARN_CXX_CMAKE_DIR ${LEARN_CXX_DIR}/cmake)

include(CTest)

message(STATUS "learn-cxx GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(LEARN_CXX_NOT_SUBPROJECT)
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
    message(STATUS "learn-cxx BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "learn-cxx BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${LEARN_CXX_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${LEARN_CXX_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${LEARN_CXX_CMAKE_DIR}/clang_tidy.cmake)
include(${LEARN_CXX_CMAKE_DIR}/sanitizer.cmake)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(argparse CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS asio beast)
find_package(doctest CONFIG REQUIRED)
find_package(esl CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Threads REQUIRED)

find_path(TL_GENERATOR_INCLUDE_DIRS "tl/generator.hpp")

target_compile_definitions(asio::asio INTERFACE
  $<$<BOOL:${WIN32}>:
    _WIN32_WINNT=0x0601
  >
)

add_subdirectory(asio)
add_subdirectory(beast)
add_subdirectory(concurrency)
add_subdirectory(coroutine)
add_subdirectory(error_code_condition)
add_subdirectory(ranges)
