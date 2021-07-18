CPMAddPackage("gh:catchorg/Catch2@2.13.4")

add_executable(backoff_retry_test)

target_sources(backoff_retry_test
  PRIVATE
    main.cpp
    backoff_test.cpp
)

apply_backoff_retry_compile_conf(backoff_retry_test)

target_link_libraries(backoff_retry_test
  PRIVATE
    backoff
    Catch2
)

get_target_property(test_FILES backoff_retry_test SOURCES)
source_group("tests" FILES ${test_FILES})

add_test(NAME test_backoff_retry COMMAND backoff_retry_test)
