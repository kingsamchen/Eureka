
add_executable(cpp14_iostream_http_cli http_client.cpp)

target_link_libraries(cpp14_iostream_http_cli PRIVATE Boost::asio)
