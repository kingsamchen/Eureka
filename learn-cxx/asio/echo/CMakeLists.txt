
add_executable(echo_srv echo_server.cpp)
learn_cxx_common_compile_configs(echo_srv)
target_link_libraries(echo_srv PRIVATE asio::asio gflags::gflags glog::glog)

add_executable(echo_cli echo_client.cpp)
learn_cxx_common_compile_configs(echo_cli)
target_link_libraries(echo_cli PRIVATE asio::asio argparse::argparse glog::glog)
