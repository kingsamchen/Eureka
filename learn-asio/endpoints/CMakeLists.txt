
add_executable(endpoints main.cpp)

apply_learn_asio_compile_conf(endpoints)

target_link_libraries(endpoints
  PRIVATE
    asio
    kbase
)
