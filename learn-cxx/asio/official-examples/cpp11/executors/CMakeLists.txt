
add_executable(cpp11_ex_actor actor.cpp)

target_link_libraries(cpp11_ex_actor
  PRIVATE
    Boost::asio
)

add_executable(cpp11_ex_bank_account_1 bank_account_1.cpp)

target_link_libraries(cpp11_ex_bank_account_1
  PRIVATE
    Boost::asio
)

add_executable(cpp11_ex_bank_account_2 bank_account_2.cpp)

target_link_libraries(cpp11_ex_bank_account_2
  PRIVATE
    Boost::asio
)

add_executable(cpp11_ex_fork_join fork_join.cpp)

target_link_libraries(cpp11_ex_fork_join
  PRIVATE
    Boost::asio
)

add_executable(cpp11_ex_pipeline pipeline.cpp)

target_link_libraries(cpp11_ex_pipeline
  PRIVATE
    Boost::asio
)

add_executable(cpp11_ex_priority_scheduler priority_scheduler.cpp)

target_link_libraries(cpp11_ex_priority_scheduler
  PRIVATE
    Boost::asio
)
