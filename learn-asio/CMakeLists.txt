cmake_minimum_required(VERSION 3.14)

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(LEARN_ASIO_NOT_SUBPROJECT ON)
endif()

project(learn-asio CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(LEARN_ASIO_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

set(LEARN_ASIO_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LEARN_ASIO_CMAKE_DIR ${LEARN_ASIO_DIR}/cmake)

include(${LEARN_ASIO_CMAKE_DIR}/CPM.cmake)

include(${LEARN_ASIO_CMAKE_DIR}/cotire.cmake)
set(LEARN_ASIO_PCH_HEADER ${LEARN_ASIO_DIR}/build/pch/precompile.h)

# Compiler and output configurations.
if(MSVC)
  string (REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  include(${LEARN_ASIO_CMAKE_DIR}/compiler_msvc.cmake)
  foreach(OUTPUTCONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG_TYPE} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG_TYPE}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG_TYPE}/lib)
  endforeach()
else()
  include(${LEARN_ASIO_CMAKE_DIR}/compiler_posix.cmake)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

if(NOT MSVC)
  message(STATUS "BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
endif()

CPMAddPackage(
  NAME KBase
  GITHUB_REPOSITORY kingsamchen/KBase
  VERSION 0.1.5
  GIT_SHALLOW TRUE
)

find_package(Threads REQUIRED)

CPMAddPackage(
  NAME asio
  GITHUB_REPOSITORY chriskohlhoff/asio
  GIT_TAG asio-1-16-1
  GIT_SHALLOW TRUE
)

if(asio_ADDED)
  add_library(asio INTERFACE)

  target_include_directories(asio
    INTERFACE ${asio_SOURCE_DIR}/asio/include
  )

  target_compile_definitions(asio
    INTERFACE
      ASIO_STANDALONE
      ASIO_NO_DEPRECATED
  )

  target_link_libraries(asio
    INTERFACE
      Threads::Threads
  )

  if(WIN32)
    # macro see @ https://stackoverflow.com/a/40217291/1746503
    macro(get_win32_winnt version)
      if (CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
        string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
        # Check for Windows 10, b/c we'll need to convert to hex 'A'.
        if ("${verMajor}" MATCHES "10")
          set(verMajor "A")
          string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
        endif ("${verMajor}" MATCHES "10")
        # Remove all remaining '.' characters.
        string(REPLACE "." "" ver ${ver})
        # Prepend each digit with a zero.
        string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
        set(${version} "0x${ver}")
      endif()
    endmacro()

    if(NOT DEFINED _WIN32_WINNT)
      get_win32_winnt(ver)
      set(_WIN32_WINNT ${ver})
    endif()

    message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

    target_compile_definitions(asio
      INTERFACE
        _WIN32_WINNT=${_WIN32_WINNT}
        WIN32_LEAN_AND_MEAN
    )
  endif()
endif()

add_subdirectory(timer)

add_subdirectory(daytime)

add_subdirectory(echo)

if(UNIX)
  add_subdirectory(handling_signal)
endif()

add_subdirectory(socks4a)
