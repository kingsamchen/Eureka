
add_executable(hashids-cli)

target_sources(hashids-cli
  PRIVATE
    main.cpp
)

target_include_directories(hashids-cli
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(hashids-cli
  PRIVATE
    argparse
    fmt

    esl
    hashids
)

hashids_apply_common_compile_options(hashids-cli)

if(HASHIDS_ENABLE_CLANG_TIDY)
  hashids_apply_clang_tidy(hashids-cli)
endif()

if(MSVC)
  if(HASHIDS_USE_MSVC_PARALLEL_BUILD)
    hashids_apply_msvc_parallel_build(hashids-cli)
  endif()
  if(HASHIDS_USE_MSVC_STATIC_ANALYSIS)
    hashids_apply_msvc_static_analysis(hashids-cli
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(HASHIDS_USE_SANITIZER)
    hashids_apply_sanitizer(hashids-cli)
  endif()
endif()

get_target_property(hashidscli_FILES hashids-cli SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${hashidscli_FILES})
