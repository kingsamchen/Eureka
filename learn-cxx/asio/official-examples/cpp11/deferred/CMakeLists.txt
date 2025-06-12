
add_executable(cpp11_deferred_1 deferred_1.cpp)

target_link_libraries(cpp11_deferred_1
  PRIVATE
    Boost::asio
)

add_executable(cpp11_deferred_2 deferred_2.cpp)

target_link_libraries(cpp11_deferred_2
  PRIVATE
    Boost::asio
    fmt::fmt
)
