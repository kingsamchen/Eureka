# set HTTP_PROXY=http://myproxy:8080 & set HTTPS_PROXY=http://myproxy:8080

cmake_minimum_required(VERSION 3.12)

project(asio-cmake)

set(ASIO_CMAKE_ASIO_TAG asio-1-12-2)

include(FetchContent)

FetchContent_Declare(
  asio
  GIT_REPOSITORY  https://github.com/chriskohlhoff/asio.git
  GIT_TAG         ${ASIO_CMAKE_ASIO_TAG}
  GIT_SHALLOW     TRUE
)

FetchContent_GetProperties(asio)

if(NOT asio_POPULATED)
  message(STATUS "Fetching asio")
  FetchContent_Populate(asio)
  message(STATUS "Fetching asio - done; src @ ${asio_SOURCE_DIR}")
  set(ASIO_SOURCE ${asio_SOURCE_DIR})
endif()

add_library(asio INTERFACE)

target_compile_definitions(asio
INTERFACE
  ASIO_STANDALONE
)

target_include_directories(asio
  INTERFACE ${ASIO_SOURCE}/asio/include
)
