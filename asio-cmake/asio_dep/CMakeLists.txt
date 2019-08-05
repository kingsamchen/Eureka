cmake_minimum_required(VERSION 3.12)

project(asio-cmake)

if(NOT DEFINED ASIO_CMAKE_ASIO_TAG)
  message(FATAL_ERROR "You must specify ASIO_CMAKE_ASIO_TAG!")
endif()

if(NOT DEFINED ASIO_CMAKE_ASIO_SOURCE_DIR)
  set(ASIO_CMAKE_ASIO_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

set(ASIO_CMAKE_ASIO_DEP_DIR "${ASIO_CMAKE_ASIO_SOURCE_DIR}/asio-${ASIO_CMAKE_ASIO_TAG}-src")

include(FetchContent)

FetchContent_Declare(asio
  GIT_REPOSITORY  https://github.com/chriskohlhoff/asio.git
  GIT_TAG         ${ASIO_CMAKE_ASIO_TAG}
  GIT_SHALLOW     TRUE
  SOURCE_DIR      ${ASIO_CMAKE_ASIO_DEP_DIR}
)

FetchContent_GetProperties(asio)

if(NOT asio_POPULATED)
  message(STATUS "Fetching asio ${ASIO_CMAKE_ASIO_TAG}")
  FetchContent_Populate(asio)
  message(STATUS "Fetching asio ${ASIO_CMAKE_ASIO_TAG} done; src @ ${asio_SOURCE_DIR}")
endif()

add_library(asio INTERFACE)

target_include_directories(asio
  INTERFACE ${asio_SOURCE_DIR}/asio/include
)

target_compile_definitions(asio
  INTERFACE ASIO_STANDALONE
)

if(UNIX)
  target_link_libraries(asio
    INTERFACE pthread
  )
endif()