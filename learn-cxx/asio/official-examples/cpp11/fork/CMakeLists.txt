
add_executable(cpp11_daemon daemon.cpp)

target_link_libraries(cpp11_daemon
  PRIVATE
    Boost::asio
)

add_executable(cpp11_proc_per_conn process_per_connection.cpp)

target_link_libraries(cpp11_proc_per_conn
  PRIVATE
    Boost::asio
)
