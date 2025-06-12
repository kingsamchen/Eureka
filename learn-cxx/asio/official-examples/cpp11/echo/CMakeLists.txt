
add_executable(cpp11_async_tcp_echo_svr async_tcp_echo_server.cpp)

target_link_libraries(cpp11_async_tcp_echo_svr
  PRIVATE
    Boost::asio
)

add_executable(cpp11_async_udp_echo_svr async_udp_echo_server.cpp)

target_link_libraries(cpp11_async_udp_echo_svr
  PRIVATE
    Boost::asio
)

add_executable(cpp11_blocking_tcp_echo_svr blocking_tcp_echo_server.cpp)

target_link_libraries(cpp11_blocking_tcp_echo_svr
  PRIVATE
    Boost::asio
)

add_executable(cpp11_blocking_tcp_echo_cli blocking_tcp_echo_client.cpp)

target_link_libraries(cpp11_blocking_tcp_echo_cli
  PRIVATE
    Boost::asio
)

add_executable(cpp11_blocking_udp_echo_svr blocking_udp_echo_server.cpp)

target_link_libraries(cpp11_blocking_udp_echo_svr
  PRIVATE
    Boost::asio
)

add_executable(cpp11_blocking_udp_echo_cli blocking_udp_echo_client.cpp)

target_link_libraries(cpp11_blocking_udp_echo_cli
  PRIVATE
    Boost::asio
)