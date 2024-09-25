
add_executable(thread_pool)

target_sources(thread_pool
  PRIVATE
    main.cpp
    task.h
    thread_pool.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(thread_pool
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(thread_pool
  PRIVATE
    doctest::doctest
    fmt::fmt
)

learn_coroutine_apply_common_compile_options(thread_pool)

if(LEARN_COROUTINE_USE_CLANG_TIDY)
  learn_coroutine_apply_clang_tidy(thread_pool)
endif()

if(LEARN_COROUTINE_USE_SANITIZER)
  learn_coroutine_apply_sanitizer(thread_pool)
endif()

# TODO: Edit at your will
if(MSVC)
  if(LEARN_COROUTINE_USE_MSVC_PARALLEL_BUILD)
    learn_coroutine_apply_msvc_parallel_build(thread_pool)
  endif()
  if(LEARN_COROUTINE_USE_MSVC_STATIC_ANALYSIS)
    learn_coroutine_apply_msvc_static_analysis(thread_pool
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
endif()

get_target_property(thread_pool_FILES thread_pool SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${thread_pool_FILES})
