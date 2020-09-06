
add_executable(alias)

target_sources(alias
  PRIVATE
    main.cpp
    tagged_bool.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(alias
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(alias
)

apply_alias_types_compile_conf(alias)

get_target_property(alias_FILES alias SOURCES)
source_group("alias" FILES ${alias_FILES})

if(MSVC AND ALIAS_TYPES_ENABLE_CODE_ANALYSIS)
  enable_alias_types_msvc_static_analysis_conf(alias
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()

