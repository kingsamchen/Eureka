
add_executable(coro_task)

target_sources(coro_task
  PRIVATE
    main.cpp
    future.h
    job.h
    job2.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(coro_task
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(coro_task
  PRIVATE
    fmt::fmt
)

learn_coroutine_apply_common_compile_options(coro_task)

if(LEARN_COROUTINE_USE_CLANG_TIDY)
  learn_coroutine_apply_clang_tidy(coro_task)
endif()

if(LEARN_COROUTINE_USE_SANITIZER)
  learn_coroutine_apply_sanitizer(coro_task)
endif()

# TODO: Edit at your will
if(MSVC)
  if(LEARN_COROUTINE_USE_MSVC_PARALLEL_BUILD)
    learn_coroutine_apply_msvc_parallel_build(coro_task)
  endif()
  if(LEARN_COROUTINE_USE_MSVC_STATIC_ANALYSIS)
    learn_coroutine_apply_msvc_static_analysis(coro_task
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
endif()

get_target_property(coro_task_FILES coro_task SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${coro_task_FILES})
