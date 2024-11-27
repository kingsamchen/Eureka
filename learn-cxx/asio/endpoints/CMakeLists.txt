
add_executable(endpoints main.cpp)

learn_cxx_apply_common_compile_options(endpoints)

target_link_libraries(endpoints
  PRIVATE
    asio::asio
)
