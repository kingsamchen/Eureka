
add_executable(cpp11_connect_pair connect_pair.cpp)

target_link_libraries(cpp11_connect_pair
  PRIVATE
    Boost::asio
    Boost::thread
)

add_executable(cpp11_fd_passing_steram_cli fd_passing_stream_client.cpp)

target_link_libraries(cpp11_fd_passing_steram_cli
  PRIVATE
    Boost::asio
)

add_executable(cpp11_fd_passing_steram_svr fd_passing_stream_server.cpp)

target_link_libraries(cpp11_fd_passing_steram_svr
  PRIVATE
    Boost::asio
)

add_executable(cpp11_iostream_client iostream_client.cpp)

target_link_libraries(cpp11_iostream_client
  PRIVATE
    Boost::asio
)

add_executable(cpp11_steram_cli stream_client.cpp)

target_link_libraries(cpp11_steram_cli
  PRIVATE
    Boost::asio
)

add_executable(cpp11_steram_svr stream_server.cpp)

target_link_libraries(cpp11_steram_svr
  PRIVATE
    Boost::asio
)
