
add_executable(endpoints main.cpp)

learn_cxx_common_compile_configs(endpoints)

target_link_libraries(endpoints
  PRIVATE
    asio::asio
)
