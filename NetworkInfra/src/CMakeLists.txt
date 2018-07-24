
set(util_SRCS
    acceptor.cpp
    buffer.cpp
    channel.cpp
    event_loop.cpp
    poller.cpp
    socket_address.cpp
    tcp_connection.cpp
    tcp_server.cpp
    timer.cpp
    timer_queue.cpp
)

set(test_SRCS
    tests/main.cpp
    tests/acceptor_unittest.cpp
    tests/buffer_unittest.cpp
    tests/reactor_unittest.cpp
    tests/socket_address_unittest.cpp
    tests/tcp_server_unittest.cpp
)

add_executable(network_utils ${util_SRCS} ${test_SRCS})

target_include_directories(network_utils
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests
    PRIVATE ${KBASE_INCLUDE_DIR}
)

add_library(kbase STATIC IMPORTED)
set_property(TARGET kbase PROPERTY IMPORTED_LOCATION ${KBASE_LIB_DIR}/libkbase.a)

target_link_libraries(network_utils kbase gtest pthread)
