
add_executable(try-ranges)

target_sources(try-ranges
  PRIVATE
    main.cpp
    chain_view.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(try-ranges
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(try-ranges
  PRIVATE
    fmt::fmt
)

learn_cxx_ranges_apply_common_compile_options(try-ranges)

if(LEARN_CXX_RANGES_ENABLE_CLANG_TIDY)
  learn_cxx_ranges_apply_clang_tidy(try-ranges)
endif()

# TODO: Edit at your will
if(MSVC)
  if(LEARN_CXX_RANGES_USE_MSVC_PARALLEL_BUILD)
    learn_cxx_ranges_apply_msvc_parallel_build(try-ranges)
  endif()
  if(LEARN_CXX_RANGES_USE_MSVC_STATIC_ANALYSIS)
    learn_cxx_ranges_apply_msvc_static_analysis(try-ranges
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(LEARN_CXX_RANGES_USE_SANITIZER)
    learn_cxx_ranges_apply_sanitizer(try-ranges)
  endif()
endif()

get_target_property(try-ranges_FILES try-ranges SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${try-ranges_FILES})

target_precompile_headers(try-ranges
  PRIVATE "${LEARN_CXX_RANGES_PCH_HEADER}"
)