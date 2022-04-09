
add_executable(base_test)

target_sources(base_test
  PRIVATE
    subprocess_test.cpp
    test_main.cpp
)

target_link_libraries(base_test
  PRIVATE
    base
    doctest
)

lumper_apply_common_compile_options(base_test)

if(LUMPER_USE_SANITIZER)
  lumper_apply_sanitizer(base_test)
endif()

add_test(NAME tests COMMAND base_test)
