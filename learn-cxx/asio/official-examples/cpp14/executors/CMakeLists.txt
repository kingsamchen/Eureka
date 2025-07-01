
add_executable(cpp14_ex_actor actor.cpp)

target_link_libraries(cpp14_ex_actor
  PRIVATE
    Boost::asio
)

add_executable(cpp14_ex_async_1 async_1.cpp)

target_link_libraries(cpp14_ex_async_1
  PRIVATE
    Boost::asio
)

add_executable(cpp14_ex_async_2 async_2.cpp)

target_link_libraries(cpp14_ex_async_2
  PRIVATE
    Boost::asio
)

add_executable(cpp14_ex_bank_account_1 bank_account_1.cpp)

target_link_libraries(cpp14_ex_bank_account_1
  PRIVATE
    Boost::asio
)

add_executable(cpp14_ex_bank_account_2 bank_account_2.cpp)

target_link_libraries(cpp14_ex_bank_account_2
  PRIVATE
    Boost::asio
)

add_executable(cpp14_ex_fork_join fork_join.cpp)

target_link_libraries(cpp14_ex_fork_join
  PRIVATE
    Boost::asio
)

add_executable(cpp14_ex_pipeline pipeline.cpp)

target_link_libraries(cpp14_ex_pipeline
  PRIVATE
    Boost::asio
)

add_executable(cpp14_ex_priority_scheduler priority_scheduler.cpp)

target_link_libraries(cpp14_ex_priority_scheduler
  PRIVATE
    Boost::asio
)
