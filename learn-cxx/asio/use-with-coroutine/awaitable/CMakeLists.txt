
add_executable(coro_awaitable)

target_sources(coro_awaitable
  PRIVATE
    main.cpp
    with_async_initiate.cpp
)

target_link_libraries(coro_awaitable
  PRIVATE
    asio::asio
    esl::esl
    fmt::fmt
    spdlog::spdlog
)

target_compile_definitions(coro_awaitable
  PRIVATE
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG
)

target_compile_features(coro_awaitable
  PRIVATE
    cxx_std_23
)

target_include_directories(coro_awaitable
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

learn_cxx_sanitizers(coro_awaitable)
