
CPMAddPackage(
  NAME argparse
  VERSION 2.1
  GITHUB_REPOSITORY p-ranav/argparse
)

add_executable(socks5)

target_sources(socks5
  PRIVATE
    main.cpp
)

apply_learn_asio_compile_conf(socks5)

target_link_libraries(socks5
  PRIVATE
    argparse
    asio
    kbase
)
