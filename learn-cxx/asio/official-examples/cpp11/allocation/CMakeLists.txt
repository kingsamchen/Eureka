
add_executable(cpp11_allocation)

target_sources(cpp11_allocation
  PRIVATE
    server.cpp
)

target_link_libraries(cpp11_allocation
  PRIVATE
    Boost::asio
)
