
add_executable(socks4a main.cpp proxy.h proxy.cpp tunnel.h tunnel.cpp endian_utils.h)
apply_learn_asio_compile_properties_to_target(socks4a)
target_link_libraries(socks4a PRIVATE kbase asio)
