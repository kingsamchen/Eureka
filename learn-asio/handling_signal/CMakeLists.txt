
add_executable(handle_signals signal_handler.cpp)
learn_asio_apply_common_compile_options(handle_signals)
target_link_libraries(handle_signals
  PRIVATE
    asio::asio
    fmt::fmt
)
