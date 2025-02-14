
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

learn_cxx_common_compile_configs(coro_thread_pool)
learn_cxx_clang_tidy(coro_thread_pool)
learn_cxx_sanitizers(coro_thread_pool)

get_target_property(coro_thread_pool_FILES coro_thread_pool SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${coro_thread_pool_FILES})
