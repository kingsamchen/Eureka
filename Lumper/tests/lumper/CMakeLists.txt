CPMAddPackage("gh:p-ranav/argparse#v2.2")

add_executable(lumper_test)

target_sources(lumper_test
  PRIVATE
    ../../lumper/cli.cpp
    ../../lumper/cgroups/util.cpp
    cgroups/util_test.cpp
    cli_test.cpp
    test_main.cpp
)

target_include_directories(lumper_test
  PRIVATE
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../
)

target_link_libraries(lumper_test
  PRIVATE
    argparse
    doctest
    esl
    fmt
)

lumper_apply_common_compile_options(lumper_test)

if(LUMPER_USE_SANITIZER)
  lumper_apply_sanitizer(lumper_test)
endif()

add_test(NAME tests COMMAND lumper_test)
