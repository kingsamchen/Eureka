
add_executable(echo_srv echo_server.cpp)
apply_learn_asio_compile_conf(echo_srv)
target_link_libraries(echo_srv PRIVATE kbase asio)

add_executable(echo_cli echo_client.cpp)
apply_learn_asio_compile_conf(echo_cli)
target_link_libraries(echo_cli PRIVATE kbase asio)
