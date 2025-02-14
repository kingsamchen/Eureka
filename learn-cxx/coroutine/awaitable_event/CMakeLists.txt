
add_executable(awaitable_event)

target_sources(awaitable_event
  PRIVATE
    test_main.cpp

    event.cpp
    event.h
    event_poller.cpp
    event_poller.h
    noreturn.h
)

target_include_directories(awaitable_event
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
  PRIVATE ${TL_GENERATOR_INCLUDE_DIRS}
)

target_link_libraries(awaitable_event
  PRIVATE
    doctest::doctest
    esl::esl
    fmt::fmt
)

learn_cxx_common_compile_configs(awaitable_event)

if(LEARN_CXX_USE_SANITIZERS)
  learn_cxx_sanitizers(awaitable_event)
endif()
