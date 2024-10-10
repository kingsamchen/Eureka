
add_executable(coro_thread_pool)

target_sources(coro_thread_pool
  PRIVATE
    main.cpp
    task.h
    thread_pool.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(coro_thread_pool
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(coro_thread_pool
  PRIVATE
    doctest::doctest
    fmt::fmt
)

learn_cxx_apply_common_compile_options(coro_thread_pool)

if(LEARN_CXX_USE_CLANG_TIDY)
  learn_cxx_apply_clang_tidy(coro_thread_pool)
endif()

if(LEARN_CXX_USE_SANITIZER)
  learn_cxx_apply_sanitizer(coro_thread_pool)
endif()

# TODO: Edit at your will
if(MSVC)
  if(LEARN_CXX_USE_MSVC_PARALLEL_BUILD)
    learn_cxx_apply_msvc_parallel_build(coro_thread_pool)
  endif()
  if(LEARN_CXX_USE_MSVC_STATIC_ANALYSIS)
    learn_cxx_apply_msvc_static_analysis(coro_thread_pool
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
endif()

get_target_property(coro_thread_pool_FILES coro_thread_pool SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${coro_thread_pool_FILES})
