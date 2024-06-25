
add_executable(simple_generator)

target_sources(simple_generator
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(simple_generator
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(simple_generator
)

learn_coroutine_apply_common_compile_options(simple_generator)

if(LEARN_COROUTINE_USE_CLANG_TIDY)
  learn_coroutine_apply_clang_tidy(simple_generator)
endif()

if(LEARN_COROUTINE_USE_SANITIZER)
  learn_coroutine_apply_sanitizer(simple_generator)
endif()

# TODO: Edit at your will
if(MSVC)
  if(LEARN_COROUTINE_USE_MSVC_PARALLEL_BUILD)
    learn_coroutine_apply_msvc_parallel_build(simple_generator)
  endif()
  if(LEARN_COROUTINE_USE_MSVC_STATIC_ANALYSIS)
    learn_coroutine_apply_msvc_static_analysis(simple_generator
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
endif()

get_target_property(simple_generator_FILES simple_generator SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${simple_generator_FILES})
