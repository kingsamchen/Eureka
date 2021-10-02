
add_library(rcp)

target_sources(rcp
  PRIVATE
    rcp.h
    pipe.h
    unique_handle.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
    rcp_posix.cpp
  >
)

target_include_directories(rcp
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(rcp
)

apply_run_child_process_compile_conf(rcp)

get_target_property(rcp_FILES rcp SOURCES)
source_group("rcp" FILES ${rcp_FILES})

if(MSVC AND RUN_CHILD_PROCESS_ENABLE_CODE_ANALYSIS)
  enable_run_child_process_msvc_static_analysis_conf(rcp
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()

set_target_properties(rcp PROPERTIES
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

