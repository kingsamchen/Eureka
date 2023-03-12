CPMAddPackage("gh:onqtam/doctest#v2.4.9")
CPMAddPackage("gh:kingsamchen/esl#ea4e9f76a47552c6d87247c4236c7a4891cee492")

add_executable(esld_test)

target_sources(esld_test
  PRIVATE
    test_main.cpp
)

target_link_libraries(esld_test
  PRIVATE
    doctest
    esld
    esl
)

esld_apply_common_compile_options(esld_test)

if(ESLD_ENABLE_CLANG_TIDY)
  esld_apply_clang_tidy(esld_test)
endif()

if(MSVC)
  if(ESLD_USE_MSVC_PARALLEL_BUILD)
    esld_apply_msvc_parallel_build(esld_test)
  endif()
  if(ESLD_USE_MSVC_STATIC_ANALYSIS)
    esld_apply_msvc_static_analysis(esld_test)
  endif()
else()
  if(ESLD_USE_SANITIZER)
    esld_apply_sanitizer(esld_test)
  endif()
endif()

get_target_property(test_FILES esld_test SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${test_FILES})

add_test(NAME tests COMMAND esld_test)

#
# perf_test
#

add_executable(perf_test)

target_sources(perf_test
  PRIVATE
    perf_main.cpp
)

target_link_libraries(perf_test
  PRIVATE
    esld
    esl
    fmt
)

esld_apply_common_compile_options(perf_test)
