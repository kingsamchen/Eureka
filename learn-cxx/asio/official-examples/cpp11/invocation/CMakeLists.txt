
add_executable(cpp11_invocation prioritized_handlers.cpp)

target_link_libraries(cpp11_invocation
  PRIVATE
    Boost::asio
)
