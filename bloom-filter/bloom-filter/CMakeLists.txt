CPMAddPackage("gh:onqtam/doctest#v2.4.8")

add_executable(bloom-filter)

target_sources(bloom-filter
  PRIVATE
    test_main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(bloom-filter
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(bloom-filter
  PRIVATE
    doctest
)

bloom_filter_apply_common_compile_options(bloom-filter)

if(MSVC)
  if(BLOOM_FILTER_USE_MSVC_PARALLEL_BUILD)
    message(STATUS "USE_MSVC_PARALLEL_BUILD is ON")
    bloom_filter_apply_msvc_parallel_build(bloom-filter)
  endif()
  if(BLOOM_FILTER_USE_MSVC_STATIC_ANALYSIS)
    message(STATUS "USE_MSVC_STATIC_ANALYSIS is ON")
    bloom_filter_apply_msvc_static_analysis(bloom-filter
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
  get_target_property(bloom-filter_FILES bloom-filter SOURCES)
  source_group("bloom-filter" FILES ${bloom-filter_FILES})
else()
  if(BLOOM_FILTER_USE_SANITIZER)
    message(STATUS "USE_SANITIZER is ON")
    bloom_filter_apply_sanitizer(bloom-filter)
  endif()
endif()

add_test(NAME tests COMMAND bloom-filter)
