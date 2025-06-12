
add_executable(cpp11_http_async_cli async_client.cpp)

target_link_libraries(cpp11_http_async_cli
  PRIVATE
    Boost::asio
)

add_executable(cpp11_http_sync_cli sync_client.cpp)

target_link_libraries(cpp11_http_sync_cli
  PRIVATE
    Boost::asio
)
