
CPMAddPackage(
  NAME kbase
  GITHUB_REPOSITORY kingsamchen/KBase
  # non-stable release
  VERSION 0.3.0
  GIT_TAG 5b6ba9a4a56a1725fe4bc598236384253c9c177b
)

CPMAddPackage(
  NAME nana
  GITHUB_REPOSITORY cnjinhao/nana
  VERSION 1.7.4
  OPTIONS
    "MSVC_USE_STATIC_RUNTIME OFF"
)

add_executable(context-menu-cleaner)

target_sources(context-menu-cleaner
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(context-menu-cleaner
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(context-menu-cleaner
  PRIVATE
    kbase
    nana
)

apply_contextmenucleaner_compile_conf(context-menu-cleaner)

get_target_property(context-menu-cleaner_FILES context-menu-cleaner SOURCES)
source_group("context-menu-cleaner" FILES ${context-menu-cleaner_FILES})

if(MSVC AND CONTEXTMENUCLEANER_ENABLE_CODE_ANALYSIS)
  enable_contextmenucleaner_msvc_static_analysis_conf(context-menu-cleaner
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()

set_target_properties(context-menu-cleaner PROPERTIES
  COTIRE_CXX_PREFIX_HEADER_INIT "${CONTEXTMENUCLEANER_PCH_HEADER}"
)
