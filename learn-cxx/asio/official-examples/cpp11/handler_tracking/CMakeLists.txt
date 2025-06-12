
add_executable(cpp11_tracking_async_echo_svr async_tcp_echo_server.cpp)

target_link_libraries(cpp11_tracking_async_echo_svr
  PRIVATE
    Boost::asio
)

target_compile_definitions(cpp11_tracking_async_echo_svr
  PRIVATE
    BOOST_ASIO_ENABLE_HANDLER_TRACKING
)

add_library(cpp11_tracking_custom INTERFACE custom_tracking.hpp )

target_link_libraries(cpp11_tracking_custom
  INTERFACE
    Boost::asio
)
