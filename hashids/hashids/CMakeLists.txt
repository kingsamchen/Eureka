
add_library(hashids)

target_sources(hashids
  PRIVATE
    hashids.cpp
    hashids.h
)

target_include_directories(hashids
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(hashids
  PRIVATE
    esl
    fmt
)

hashids_apply_common_compile_options(hashids)

if(HASHIDS_ENABLE_CLANG_TIDY)
  hashids_apply_clang_tidy(hashids)
endif()

if(MSVC)
  if(HASHIDS_USE_MSVC_PARALLEL_BUILD)
    hashids_apply_msvc_parallel_build(hashids)
  endif()
  if(HASHIDS_USE_MSVC_STATIC_ANALYSIS)
    hashids_apply_msvc_static_analysis(hashids
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(HASHIDS_USE_SANITIZER)
    hashids_apply_sanitizer(hashids)
  endif()
endif()

get_target_property(hashids_FILES hashids SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${hashids_FILES})
