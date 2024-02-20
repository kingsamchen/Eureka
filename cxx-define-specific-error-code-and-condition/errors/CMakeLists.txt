
add_executable(errors)

target_sources(errors
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(errors
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(errors
  PRIVATE
    fmt::fmt
)

my_errors_apply_common_compile_options(errors)

if(MY_ERRORS_ENABLE_CLANG_TIDY)
  my_errors_apply_clang_tidy(errors)
endif()

# TODO: Edit at your will
if(MSVC)
  if(MY_ERRORS_USE_MSVC_PARALLEL_BUILD)
    my_errors_apply_msvc_parallel_build(errors)
  endif()
  if(MY_ERRORS_USE_MSVC_STATIC_ANALYSIS)
    my_errors_apply_msvc_static_analysis(errors
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(MY_ERRORS_USE_SANITIZER)
    my_errors_apply_sanitizer(errors)
  endif()
endif()

get_target_property(errors_FILES errors SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${errors_FILES})
