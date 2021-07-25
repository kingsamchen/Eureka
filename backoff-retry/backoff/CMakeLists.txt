
add_library(backoff)

target_sources(backoff
  PRIVATE
    backoff.cpp
    backoff.h
    backoff_policy.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(backoff
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(backoff
)

apply_backoff_retry_compile_conf(backoff)

get_target_property(backoff_FILES backoff SOURCES)
source_group("backoff" FILES ${backoff_FILES})

if(MSVC AND BACKOFF_RETRY_ENABLE_CODE_ANALYSIS)
  enable_backoff_retry_msvc_static_analysis_conf(backoff
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()

set_target_properties(backoff PROPERTIES
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

