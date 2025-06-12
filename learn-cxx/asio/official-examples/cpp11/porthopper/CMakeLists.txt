
add_executable(cpp11_porthopper_cli client.cpp)
target_link_libraries(cpp11_porthopper_cli Boost::asio)

add_executable(cpp11_porthopper_srv server.cpp)
target_link_libraries(cpp11_porthopper_srv Boost::asio)
