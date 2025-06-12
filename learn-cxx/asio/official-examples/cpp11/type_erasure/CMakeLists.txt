
add_executable(cpp11_type_erasure main.cpp sleep.cpp stdin_line_reader.cpp)
target_link_libraries(cpp11_type_erasure Boost::asio)
