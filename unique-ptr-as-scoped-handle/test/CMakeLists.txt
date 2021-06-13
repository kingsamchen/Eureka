CPMAddPackage("gh:catchorg/Catch2@2.13.4")

add_executable(all_in_one_test)

target_sources(all_in_one_test
  PRIVATE
    main.cpp
    scoped_handle_test.cpp
)

apply_scoped_handle_compile_conf(all_in_one_test)

target_compile_definitions(all_in_one_test
  PRIVATE
    $<$<BOOL:${MSVC}>:
      _CRT_SECURE_NO_WARNINGS
    >
)

target_link_libraries(all_in_one_test
  PRIVATE
    scopedhandle
    Catch2
)

get_target_property(test_FILES all_in_one_test SOURCES)
source_group("test" FILES ${test_FILES})

add_test(NAME all_in_one_test COMMAND all_in_one_test)

set_target_properties(all_in_one_test PROPERTIES
    VS_GLOBAL_RunCodeAnalysis true

    # Use visual studio core guidelines
    # Tweak as your wish.
    VS_GLOBAL_EnableMicrosoftCodeAnalysis true
    # VS_GLOBAL_CodeAnalysisRuleSet ${CMAKE_CURRENT_SOURCE_DIR}/foo.ruleset
    # VS_GLOBAL_CodeAnalysisRuleSet ${CMAKE_CURRENT_SOURCE_DIR}/foo.ruleset

    # Use clangtidy
    # Tweak as your wish.
    VS_GLOBAL_EnableClangTidyCodeAnalysis true
    # VS_GLOBAL_ClangTidyChecks -checks=-*,modernize-*,-modernize-use-trailing-return-type
)
