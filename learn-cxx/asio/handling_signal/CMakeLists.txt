
add_executable(handle_signals signal_handler.cpp)
learn_cxx_common_compile_configs(handle_signals)
target_link_libraries(handle_signals
  PRIVATE
    asio::asio
    fmt::fmt
)
