
add_executable(coro_cancellation)

target_sources(coro_cancellation
  PRIVATE
    main.cpp
)

target_link_libraries(coro_cancellation
  PRIVATE
    Boost::asio
    esl::esl
    fmt::fmt
    spdlog::spdlog
    doctest::doctest
)

target_compile_definitions(coro_cancellation
  PRIVATE
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG
)

target_compile_features(coro_cancellation
  PRIVATE
    cxx_std_23
)

target_include_directories(coro_cancellation
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

learn_cxx_sanitizers(coro_cancellation)
