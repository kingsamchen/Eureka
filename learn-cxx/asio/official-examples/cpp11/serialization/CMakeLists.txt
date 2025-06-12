
add_executable(cpp11_serde_client client.cpp)
target_link_libraries(cpp11_serde_client Boost::asio Boost::serialization)

add_executable(cpp11_serde_server server.cpp)
target_link_libraries(cpp11_serde_server Boost::asio Boost::serialization)
