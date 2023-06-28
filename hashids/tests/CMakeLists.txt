
add_executable(hashids_test)

target_sources(hashids_test
  PRIVATE
    test_main.cpp
    hashids_test.cpp
    hashids_internals_test.cpp
)

target_link_libraries(hashids_test
  PRIVATE
    doctest
    esl
    fmt
    hashids
)

hashids_apply_common_compile_options(hashids_test)

if(HASHIDS_ENABLE_CLANG_TIDY)
  hashids_apply_clang_tidy(hashids_test)
endif()

if(MSVC)
  if(HASHIDS_USE_MSVC_PARALLEL_BUILD)
    hashids_apply_msvc_parallel_build(hashids_test)
  endif()
  if(HASHIDS_USE_MSVC_STATIC_ANALYSIS)
    hashids_apply_msvc_static_analysis(hashids_test)
  endif()
else()
  if(HASHIDS_USE_SANITIZER)
    hashids_apply_sanitizer(hashids_test)
  endif()
endif()

get_target_property(test_FILES hashids_test SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${test_FILES})

add_test(NAME tests COMMAND hashids_test)