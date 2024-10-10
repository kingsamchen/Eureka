
add_executable(error_code_condition)

target_sources(error_code_condition
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(error_code_condition
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(error_code_condition
  PRIVATE
    fmt::fmt
)

learn_cxx_apply_common_compile_options(error_code_condition)

if(LEARN_CXX_ENABLE_CLANG_TIDY)
  learn_cxx_apply_clang_tidy(error_code_condition)
endif()

# TODO: Edit at your will
if(MSVC)
  if(LEARN_CXX_USE_MSVC_PARALLEL_BUILD)
    learn_cxx_apply_msvc_parallel_build(error_code_condition)
  endif()
  if(LEARN_CXX_USE_MSVC_STATIC_ANALYSIS)
    learn_cxx_apply_msvc_static_analysis(error_code_condition
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(LEARN_CXX_USE_SANITIZER)
    learn_cxx_apply_sanitizer(error_code_condition)
  endif()
endif()

get_target_property(error_code_condition_FILES error_code_condition SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${error_code_condition_FILES})
