
add_executable(cpp11_multicast_sender sender.cpp)

target_link_libraries(cpp11_multicast_sender Boost::asio)

add_executable(cpp11_multicast_receiver receiver.cpp)

target_link_libraries(cpp11_multicast_receiver Boost::asio)