
add_executable(tcp_daytime_srv tcp_daytime_server.cpp)
learn_cxx_common_compile_configs(tcp_daytime_srv)
target_link_libraries(tcp_daytime_srv PRIVATE gflags::gflags asio::asio)

add_executable(tcp_daytime_cli tcp_daytime_client.cpp)
learn_cxx_common_compile_configs(tcp_daytime_cli)
target_link_libraries(tcp_daytime_cli PRIVATE asio::asio argparse::argparse)

add_executable(tcp_daytime_async_srv tcp_daytime_async_server.cpp)
learn_cxx_common_compile_configs(tcp_daytime_async_srv)
target_link_libraries(tcp_daytime_async_srv PRIVATE gflags::gflags asio::asio)
