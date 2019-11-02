
set(ratelimit_SRCS
  main.cpp
  bucket_limiter.cpp
)

if(WIN32)
  list(APPEND ratelimit_SRCS
  )
elseif(UNIX)
  list(APPEND ratelimit_SRCS
  )
endif()

set(ratelimit_HEADERS
  bucket_limiter.h
)

if(WIN32)
  list(APPEND ratelimit_HEADERS
  )
elseif(UNIX)
  list(APPEND ratelimit_HEADERS
  )
endif()

set(ratelimit_FILES ${ratelimit_HEADERS} ${ratelimit_SRCS})

source_group("ratelimit" FILES ${ratelimit_FILES})

add_executable(ratelimit ${ratelimit_FILES})

apply_token_bucket_rate_limit_compile_properties_to_target(ratelimit)

if(MSVC)
  enable_msvc_static_analysis_for_target(ratelimit)
endif()

target_include_directories(ratelimit
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

# Uncomment if you use them.
#target_compile_definitions(ratelimit
#)

target_link_libraries(ratelimit
)
