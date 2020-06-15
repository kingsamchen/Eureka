
add_executable(signals signal_handler.cpp)
apply_learn_asio_compile_conf(signals)
target_link_libraries(signals PRIVATE kbase asio)
