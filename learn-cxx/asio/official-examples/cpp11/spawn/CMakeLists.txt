
add_executable(cpp11_spawn_echo_svr echo_server.cpp)
target_link_libraries(cpp11_spawn_echo_svr Boost::asio)

add_executable(cpp11_spawn_pgrep parallel_grep.cpp)
target_link_libraries(cpp11_spawn_pgrep Boost::asio)
