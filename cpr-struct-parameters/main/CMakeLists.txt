
add_executable(main)

target_sources(main
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(main
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(main
  PRIVATE
    cpr::cpr
    doctest::doctest
)

cpr_struct_parameters_apply_common_compile_options(main)

if(CPR_STRUCT_PARAMETERS_USE_CLANG_TIDY)
  cpr_struct_parameters_apply_clang_tidy(main)
endif()

if(CPR_STRUCT_PARAMETERS_USE_SANITIZER)
  cpr_struct_parameters_apply_sanitizer(main)
endif()

# TODO: Edit at your will
if(MSVC)
  if(CPR_STRUCT_PARAMETERS_USE_MSVC_PARALLEL_BUILD)
    cpr_struct_parameters_apply_msvc_parallel_build(main)
  endif()
  if(CPR_STRUCT_PARAMETERS_USE_MSVC_STATIC_ANALYSIS)
    cpr_struct_parameters_apply_msvc_static_analysis(main
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()

  target_compile_options(main
    PRIVATE /Zc:preprocessor
  )
endif()

get_target_property(main_FILES main SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${main_FILES})
