
add_executable(ex_thread_pool thread_pool.cpp)

target_link_libraries(ex_thread_pool
  PRIVATE
    asio::asio
    doctest::doctest
    fmt::fmt
)

learn_cxx_common_compile_configs(ex_thread_pool)
learn_cxx_sanitizers(ex_thread_pool)
