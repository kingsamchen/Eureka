cmake_minimum_required(VERSION 3.18)

project(libeav-poc C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(libeav REQUIRED IMPORTED_TARGET libeav)

add_executable(poc)

target_sources(poc
  PRIVATE
    main.c
)

target_link_libraries(poc
  PUBLIC
    PkgConfig::libeav
)
