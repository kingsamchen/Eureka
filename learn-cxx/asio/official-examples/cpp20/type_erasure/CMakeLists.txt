
add_executable(cpp20_type_erasure main.cpp sleep.cpp stdin_line_reader.cpp)
target_link_libraries(cpp20_type_erasure Boost::asio)
