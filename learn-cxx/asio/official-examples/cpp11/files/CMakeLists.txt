
add_executable(cpp11_async_file_cp async_file_copy.cpp)

target_link_libraries(cpp11_async_file_cp
  PRIVATE
    Boost::asio
    PkgConfig::liburing
)

add_executable(cpp11_blocking_file_cp blocking_file_copy.cpp)

target_link_libraries(cpp11_blocking_file_cp
  PRIVATE
    Boost::asio
    PkgConfig::liburing
)
