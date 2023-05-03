add_executable(multipart)

target_sources(multipart
  PRIVATE
    main.cpp
    multipart.h
    multipart.cpp
)

target_include_directories(multipart
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(multipart
  PRIVATE
    doctest::doctest
    fmt::fmt
    esl
)

http_multipart_apply_common_compile_options(multipart)

if(HTTP_MULTIPART_ENABLE_CLANG_TIDY)
  http_multipart_apply_clang_tidy(multipart)
endif()

# TODO: Edit at your will
if(MSVC)
  if(HTTP_MULTIPART_USE_MSVC_PARALLEL_BUILD)
    http_multipart_apply_msvc_parallel_build(multipart)
  endif()
  if(HTTP_MULTIPART_USE_MSVC_STATIC_ANALYSIS)
    http_multipart_apply_msvc_static_analysis(multipart
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(HTTP_MULTIPART_USE_SANITIZER)
    http_multipart_apply_sanitizer(multipart)
  endif()
endif()

get_target_property(multipart_FILES multipart SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${multipart_FILES})
