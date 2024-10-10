
add_executable(coro_generator)

target_sources(coro_generator
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(coro_generator
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(coro_generator
)

learn_cxx_apply_common_compile_options(coro_generator)

if(LEARN_CXX_USE_CLANG_TIDY)
  learn_cxx_apply_clang_tidy(coro_generator)
endif()

if(LEARN_CXX_USE_SANITIZER)
  learn_cxx_apply_sanitizer(coro_generator)
endif()

# TODO: Edit at your will
if(MSVC)
  if(LEARN_CXX_USE_MSVC_PARALLEL_BUILD)
    learn_cxx_apply_msvc_parallel_build(coro_generator)
  endif()
  if(LEARN_CXX_USE_MSVC_STATIC_ANALYSIS)
    learn_cxx_apply_msvc_static_analysis(coro_generator
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
endif()

get_target_property(coro_generator_FILES coro_generator SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${coro_generator_FILES})
