
add_executable(foobar)

target_sources(foobar
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(foobar
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(foobar
  PRIVATE
    fmt::fmt
    spdlog::spdlog

    esl::esl
    uuidxx::uuidxx
)

target_compile_options(foobar
  PRIVATE
    /utf-8
)

try_vcpkg_apply_common_compile_options(foobar)

if(TRY_VCPKG_ENABLE_CLANG_TIDY)
  try_vcpkg_apply_clang_tidy(foobar)
endif()

# TODO: Edit at your will
if(MSVC)
  if(TRY_VCPKG_USE_MSVC_PARALLEL_BUILD)
    try_vcpkg_apply_msvc_parallel_build(foobar)
  endif()
  if(TRY_VCPKG_USE_MSVC_STATIC_ANALYSIS)
    try_vcpkg_apply_msvc_static_analysis(foobar
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(TRY_VCPKG_USE_SANITIZER)
    try_vcpkg_apply_sanitizer(foobar)
  endif()
endif()

get_target_property(foobar_FILES foobar SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${foobar_FILES})

target_precompile_headers(foobar
  PRIVATE "${TRY_VCPKG_PCH_HEADER}"
)