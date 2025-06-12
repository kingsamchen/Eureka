
add_executable(cpp11_use_future_daytime_cli daytime_client.cpp)

target_link_libraries(cpp11_use_future_daytime_cli
  PRIVATE
    Boost::asio
)
