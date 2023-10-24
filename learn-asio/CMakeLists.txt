cmake_minimum_required(VERSION 3.21)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(LEARN_ASIO_NOT_SUBPROJECT ON)
else()
  set(LEARN_ASIO_NOT_SUBPROJECT OFF)
endif()

message(STATUS "learn-asio as root project = ${LEARN_ASIO_NOT_SUBPROJECT}")

project(learn-asio CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(LEARN_ASIO_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(LEARN_ASIO_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LEARN_ASIO_CMAKE_DIR ${LEARN_ASIO_DIR}/cmake)

include(CTest)
include(${LEARN_ASIO_CMAKE_DIR}/CPM.cmake)

message(STATUS "learn-asio GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(LEARN_ASIO_NOT_SUBPROJECT)
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
    message(STATUS "learn-asio BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "learn-asio BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${LEARN_ASIO_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${LEARN_ASIO_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${LEARN_ASIO_CMAKE_DIR}/clang_tidy.cmake)

find_package(argparse CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(esl CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(daytime)
add_subdirectory(echo)
add_subdirectory(endpoints)
add_subdirectory(handling_signal)
add_subdirectory(socks4a)
add_subdirectory(socks5)
add_subdirectory(timer)
