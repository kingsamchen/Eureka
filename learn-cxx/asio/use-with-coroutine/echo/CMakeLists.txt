
add_executable(coro_echo_server)
target_sources(coro_echo_server PRIVATE echo_server.cpp)
target_link_libraries(coro_echo_server
  PRIVATE
    asio::asio
    fmt::fmt
    gflags::gflags
    esl::esl
)
learn_cxx_apply_common_compile_options(coro_echo_server)
if(LEARN_CXX_USE_CLANG_TIDY)
  learn_cxx_apply_common_compile_options(coro_echo_server)
endif()
if(LEARN_CXX_USE_SANITIZER)
  learn_cxx_apply_sanitizer(coro_echo_server)
endif()

add_executable(coro_echo_server2)
target_sources(coro_echo_server2 PRIVATE echo_server_with_timeout.cpp)
target_link_libraries(coro_echo_server2
  PRIVATE
    asio::asio
    fmt::fmt
    gflags::gflags
    spdlog::spdlog
)
learn_cxx_apply_common_compile_options(coro_echo_server2)
if(LEARN_CXX_USE_CLANG_TIDY)
  learn_cxx_apply_common_compile_options(coro_echo_server2)
endif()
if(LEARN_CXX_USE_SANITIZER)
  learn_cxx_apply_sanitizer(coro_echo_server)
endif()
