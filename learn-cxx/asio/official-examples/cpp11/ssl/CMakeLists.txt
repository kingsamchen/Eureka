
add_executable(cpp11_ssl_cli client.cpp)
target_link_libraries(cpp11_ssl_cli Boost::asio OpenSSL::SSL OpenSSL::Crypto)

add_executable(cpp11_ssl_svr server.cpp)
target_link_libraries(cpp11_ssl_svr Boost::asio OpenSSL::SSL OpenSSL::Crypto)
