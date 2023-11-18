
add_executable(coro_chat_server)

target_sources(coro_chat_server
  PRIVATE
    main.cpp

    chat_server.cpp
    chat_server.h
    session.cpp
    session.h
)

target_link_libraries(coro_chat_server
  PRIVATE
    asio::asio
    esl::esl
    fmt::fmt
    gflags::gflags
    spdlog::spdlog
)

target_compile_definitions(coro_chat_server
  PRIVATE
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG
)

target_include_directories(coro_chat_server
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

if(LEARN_ASIO_ENABLE_CLANG_TIDY)
  learn_asio_apply_clang_tidy(coro_chat_server)
endif()

if(LEARN_ASIO_USE_SANITIZER)
  learn_asio_apply_sanitizer(coro_chat_server)
endif()
