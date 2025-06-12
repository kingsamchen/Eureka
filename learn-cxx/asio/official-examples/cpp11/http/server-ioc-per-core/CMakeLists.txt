
add_executable(cpp11_http_server_per_ioc)

target_sources(cpp11_http_server_per_ioc
  PRIVATE
    connection.cpp
    connection.hpp
    header.hpp
    io_context_pool.cpp
    io_context_pool.hpp
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

target_link_libraries(cpp11_http_server_per_ioc
  PRIVATE
    Boost::asio
)
