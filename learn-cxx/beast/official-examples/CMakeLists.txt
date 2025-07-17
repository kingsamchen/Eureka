
add_executable(beast_http_server_async http_server_async.cpp)
target_link_libraries(beast_http_server_async
  PRIVATE
    Boost::asio
    Boost::beast
)

add_executable(beast_http_server_async_ssl http_server_async_ssl.cpp)
target_link_libraries(beast_http_server_async_ssl
  PRIVATE
    Boost::asio
    Boost::beast
    OpenSSL::SSL
    OpenSSL::Crypto
)

add_executable(beast_http_server_awaitable http_server_awaitable.cpp)
target_link_libraries(beast_http_server_awaitable
  PRIVATE
    Boost::asio
    Boost::beast
)

add_executable(beast_http_server_fast http_server_fast.cpp)
target_link_libraries(beast_http_server_fast
  PRIVATE
    Boost::asio
    Boost::beast
)

add_executable(beast_http_server_small http_server_small.cpp)
target_link_libraries(beast_http_server_small
  PRIVATE
    Boost::asio
    Boost::beast
)

add_executable(beast_http_server_flex http_server_flex.cpp)
target_link_libraries(beast_http_server_flex
  PRIVATE
    Boost::asio
    Boost::beast
    OpenSSL::SSL
    OpenSSL::Crypto
)

add_executable(beast_http_server_coro_ssl http_server_coro_ssl.cpp)
target_link_libraries(beast_http_server_coro_ssl
  PRIVATE
    Boost::asio
    Boost::beast
    OpenSSL::SSL
    OpenSSL::Crypto
)
