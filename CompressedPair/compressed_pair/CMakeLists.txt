
set(compressed_pair_SRCS
  main.cpp
)

if(WIN32)
  list(APPEND compressed_pair_SRCS
  )
elseif(UNIX)
  list(APPEND compressed_pair_SRCS
  )
endif()

set(compressed_pair_HEADERS
  compressed_pair.h
)

if(WIN32)
  list(APPEND compressed_pair_HEADERS
  )
elseif(UNIX)
  list(APPEND compressed_pair_HEADERS
  )
endif()

set(compressed_pair_FILES ${compressed_pair_HEADERS} ${compressed_pair_SRCS})

source_group("compressed_pair" FILES ${compressed_pair_FILES})

add_executable(compressed_pair ${compressed_pair_FILES})

apply_common_compile_properties_to_target(compressed_pair)

if(MSVC)
  enable_msvc_static_analysis_for_target(compressed_pair)
endif()

target_include_directories(compressed_pair
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

# Uncomment if you use them.
#target_compile_definitions(compressed_pair
#)

target_link_libraries(compressed_pair
)
