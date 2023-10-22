cmake_minimum_required(VERSION 3.21)

# Add POLICY below.

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(LOGIN_PASSWORDS_POC_NOT_SUBPROJECT ON)
else()
  set(LOGIN_PASSWORDS_POC_NOT_SUBPROJECT OFF)
endif()

message(STATUS "login-passwords-poc as root project = ${LOGIN_PASSWORDS_POC_NOT_SUBPROJECT}")

project(login-passwords-poc CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(LOGIN_PASSWORDS_POC_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

# Add options below.

set(LOGIN_PASSWORDS_POC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LOGIN_PASSWORDS_POC_CMAKE_DIR ${LOGIN_PASSWORDS_POC_DIR}/cmake)

include(CTest)

message(STATUS "login-passwords-poc GENERATOR = " ${CMAKE_GENERATOR})

set(LOGIN_PASSWORDS_POC_PCH_HEADER ${LOGIN_PASSWORDS_POC_DIR}/build/pch/precompile.h)

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(LOGIN_PASSWORDS_POC_NOT_SUBPROJECT)
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
    message(STATUS "login-passwords-poc BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "login-passwords-poc BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${LOGIN_PASSWORDS_POC_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${LOGIN_PASSWORDS_POC_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${LOGIN_PASSWORDS_POC_CMAKE_DIR}/clang_tidy.cmake)

# Declare deps
find_package(PkgConfig REQUIRED)
find_package(doctest CONFIG REQUIRED)

pkg_check_modules(botan-3 REQUIRED IMPORTED_TARGET botan-3)

add_subdirectory(main)
