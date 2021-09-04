CPMAddPackage(
  NAME nana
  GITHUB_REPOSITORY cnjinhao/nana
  VERSION 1.7.4
  OPTIONS
    "MSVC_USE_STATIC_RUNTIME OFF"
)

CPMAddPackage("gh:gabime/spdlog@1.9.2")

add_executable(himsw WIN32)

target_sources(himsw
  PRIVATE
    main.cpp
    labor_monitor.cpp
    labor_monitor.h
    win_last_error.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(himsw
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_compile_definitions(himsw
  PRIVATE
    NOMINMAX
)

target_link_libraries(himsw
  PRIVATE
    nana
    spdlog
)

apply_hey_i_am_still_working_compile_conf(himsw)

get_target_property(himsw_FILES himsw SOURCES)
source_group("himsw" FILES ${himsw_FILES})

if(MSVC AND HEY_I_AM_STILL_WORKING_ENABLE_CODE_ANALYSIS)
  enable_hey_i_am_still_working_msvc_static_analysis_conf(himsw
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()

set_target_properties(himsw PROPERTIES
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

target_precompile_headers(himsw
  PRIVATE "${HEY_I_AM_STILL_WORKING_PCH_HEADER}"
)

