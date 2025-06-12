add_executable(cpp11_chat_server chat_server.cpp chat_message.hpp)

target_link_libraries(cpp11_chat_server
  PRIVATE
    Boost::asio
)

add_executable(cpp11_chat_client chat_client.cpp chat_message.hpp)

target_link_libraries(cpp11_chat_client
  PRIVATE
    Boost::asio
)

add_executable(cpp11_posix_chat_client posix_chat_client.cpp chat_message.hpp)

target_link_libraries(cpp11_posix_chat_client
  PRIVATE
    Boost::asio
)
