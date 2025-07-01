
add_executable(cpp14_pg_parallel_sort parallel_sort.cpp)
target_link_libraries(cpp14_pg_parallel_sort Boost::asio)

add_executable(cpp14_pg_ranged_wait_all ranged_wait_for_all.cpp)
target_link_libraries(cpp14_pg_ranged_wait_all Boost::asio)

add_executable(cpp14_pg_wait_all wait_for_all.cpp)
target_link_libraries(cpp14_pg_wait_all Boost::asio)

add_executable(cpp14_pg_wait_one wait_for_one.cpp)
target_link_libraries(cpp14_pg_wait_one Boost::asio)

add_executable(cpp14_pg_wait_one_error wait_for_one_error.cpp)
target_link_libraries(cpp14_pg_wait_one_error Boost::asio)

add_executable(cpp14_pg_wait_one_success wait_for_one_success.cpp)
target_link_libraries(cpp14_pg_wait_one_success Boost::asio)
