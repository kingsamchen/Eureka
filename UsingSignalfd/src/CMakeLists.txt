
include(${USE_SIGNALFD_DEPS_DIR}/fetch_ezio.cmake)

set(use_signalfd_SRCS
  main.cpp
)

set(use_signalfd_HEADERS
)


set(use_signalfd_FILES ${use_signalfd_HEADERS} ${use_signalfd_SRCS})

source_group("src" FILES ${use_signalfd_FILES})

add_executable(use_signalfd ${use_signalfd_FILES})

apply_common_compile_properties_to_target(use_signalfd)

target_include_directories(use_signalfd
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(use_signalfd
  PRIVATE ezio kbase
)

if(UNIX)
  target_link_libraries(use_signalfd
    INTERFACE pthread)
endif()
