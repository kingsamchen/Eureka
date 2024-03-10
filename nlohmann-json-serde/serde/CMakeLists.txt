
add_executable(serde)

target_sources(serde
  PRIVATE
    json_serde.h
    json_serde_options.h

    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(serde
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${CPPCODEC_INCLUDE_DIRS}/../
)

target_link_libraries(serde
  PRIVATE
    nlohmann_json::nlohmann_json
)

json_serde_apply_common_compile_options(serde)

if(JSON_SERDE_ENABLE_CLANG_TIDY)
  json_serde_apply_clang_tidy(serde)
endif()

# TODO: Edit at your will
if(MSVC)
  if(JSON_SERDE_USE_MSVC_PARALLEL_BUILD)
    json_serde_apply_msvc_parallel_build(serde)
  endif()
  if(JSON_SERDE_USE_MSVC_STATIC_ANALYSIS)
    json_serde_apply_msvc_static_analysis(serde
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(JSON_SERDE_USE_SANITIZER)
    json_serde_apply_sanitizer(serde)
  endif()
endif()

get_target_property(serde_FILES serde SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${serde_FILES})
