
add_executable(socks4a main.cpp proxy.h proxy.cpp tunnel.h tunnel.cpp)
learn_cxx_apply_common_compile_options(socks4a)
target_link_libraries(socks4a PRIVATE asio::asio esl::esl fmt::fmt gflags::gflags glog::glog)
