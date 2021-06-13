
add_library(scopedhandle)

target_sources(scopedhandle
  PRIVATE
    scoped_handle.cpp
    scoped_handle.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(scopedhandle
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(scopedhandle
)

apply_scoped_handle_compile_conf(scopedhandle)

get_target_property(scopedhandle_FILES scopedhandle SOURCES)
source_group("scopedhandle" FILES ${scopedhandle_FILES})

if(MSVC AND SCOPED_HANDLE_ENABLE_CODE_ANALYSIS)
  enable_scoped_handle_msvc_static_analysis_conf(scopedhandle
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()

set_target_properties(scopedhandle PROPERTIES
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
