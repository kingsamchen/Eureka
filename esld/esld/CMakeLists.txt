
CPMAddPackage("gh:fmtlib/fmt#8.1.1")
CPMAddPackage("gh:kingsamchen/esl#ea4e9f76a47552c6d87247c4236c7a4891cee492")

add_library(esld)

target_sources(esld
  PRIVATE
    psl.cpp
    psl.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(esld
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(esld
  PRIVATE
    fmt
    esl
)

esld_apply_common_compile_options(esld)

if(ESLD_ENABLE_CLANG_TIDY)
  esld_apply_clang_tidy(esld)
endif()

if(MSVC)
  if(ESLD_USE_MSVC_PARALLEL_BUILD)
    esld_apply_msvc_parallel_build(esld)
  endif()
  if(ESLD_USE_MSVC_STATIC_ANALYSIS)
    esld_apply_msvc_static_analysis(esld
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(ESLD_USE_SANITIZER)
    esld_apply_sanitizer(esld)
  endif()
endif()

get_target_property(esld_FILES esld SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${esld_FILES})
