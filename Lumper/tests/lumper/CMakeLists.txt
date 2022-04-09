
add_executable(lumper_test)

target_sources(lumper_test
  PRIVATE
    test_main.cpp
)

target_link_libraries(lumper_test
  PRIVATE
    doctest
)

lumper_apply_common_compile_options(lumper_test)

if(LUMPER_USE_SANITIZER)
  lumper_apply_sanitizer(lumper_test)
endif()

add_test(NAME tests COMMAND lumper_test)
