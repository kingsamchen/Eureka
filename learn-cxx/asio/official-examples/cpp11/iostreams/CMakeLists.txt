
add_executable(cpp11_iostream_svr daytime_server.cpp)

target_link_libraries(cpp11_iostream_svr PRIVATE Boost::asio)

add_executable(cpp11_iostream_cli daytime_client.cpp)

target_link_libraries(cpp11_iostream_cli PRIVATE Boost::asio)

add_executable(cpp11_iostream_http_cli http_client.cpp)

target_link_libraries(cpp11_iostream_http_cli PRIVATE Boost::asio)
