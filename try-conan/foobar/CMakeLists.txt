
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
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(foobar
)

try_conan_common_compile_configs(foobar)

try_conan_clang_tidy_on_build(foobar)

try_conan_use_sanitizers(foobar)

get_target_property(foobar_FILES foobar SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${foobar_FILES})
