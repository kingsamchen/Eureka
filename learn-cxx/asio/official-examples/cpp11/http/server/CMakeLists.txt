
add_executable(cpp11_http_server)

target_sources(cpp11_http_server
  PRIVATE
    connection_manager.cpp
    connection_manager.hpp
    connection.cpp
    connection.hpp
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

target_link_libraries(cpp11_http_server
  PRIVATE
    Boost::asio
)
