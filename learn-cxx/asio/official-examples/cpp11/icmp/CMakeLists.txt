
add_executable(cpp11_icmp icmp_header.hpp ipv4_header.hpp ping.cpp)

target_link_libraries(cpp11_icmp
  PRIVATE
    Boost::asio
)
