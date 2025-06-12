
add_executable(cpp11_http_server_stackless_coro)

target_sources(cpp11_http_server_stackless_coro
  PRIVATE
    file_handler.cpp
    file_handler.hpp
    header.hpp
    main.cpp
    mime_types.cpp
    mime_types.hpp
    reply.cpp
    reply.hpp
    request_handler.cpp
    request_handler.hpp
    request_parser.cpp
    request_parser.hpp
    request.hpp
    server.cpp
    server.hpp
)

target_link_libraries(cpp11_http_server_stackless_coro
  PRIVATE
    Boost::asio
)
