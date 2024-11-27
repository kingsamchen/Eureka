
add_executable(socks5)

target_sources(socks5
  PRIVATE
    main.cpp
    proxy.h
    proxy.cpp
    event_thread.h
    event_thread.cpp
    tunnel.h
    tunnel.cpp
    packets.h
)

learn_cxx_apply_common_compile_options(socks5)

target_link_libraries(socks5
  PRIVATE
    argparse::argparse
    asio::asio
    esl::esl
    fmt::fmt
    glog::glog
)
