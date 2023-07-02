CPMAddPackage("gh:onqtam/doctest#v2.4.9")

add_executable(typelist)

target_sources(typelist
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(typelist
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(typelist
  PRIVATE
    doctest
)

typelist_apply_common_compile_options(typelist)

if(TYPELIST_ENABLE_CLANG_TIDY)
  typelist_apply_clang_tidy(typelist)
endif()

# TODO: Edit at your will
if(MSVC)
  if(TYPELIST_USE_MSVC_PARALLEL_BUILD)
    typelist_apply_msvc_parallel_build(typelist)
  endif()
  if(TYPELIST_USE_MSVC_STATIC_ANALYSIS)
    typelist_apply_msvc_static_analysis(typelist
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(TYPELIST_USE_SANITIZER)
    typelist_apply_sanitizer(typelist)
  endif()
endif()

get_target_property(typelist_FILES typelist SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${typelist_FILES})
