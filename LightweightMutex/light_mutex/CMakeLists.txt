
set(light_mutex_SRCS
  main.cpp
)

set(light_mutex_HEADERS
  light_mutex.h
)

set(light_mutex_FILES ${light_mutex_HEADERS} ${light_mutex_SRCS})

source_group("light_mutex" FILES ${light_mutex_FILES})

add_executable(light_mutex ${light_mutex_FILES})

apply_common_compile_properties_to_target(light_mutex)

if(MSVC)
  enable_msvc_static_analysis_for_target(light_mutex)
endif()

target_include_directories(light_mutex
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

# target_compile_definitions(light_mutex
# )

target_link_libraries(light_mutex
)
