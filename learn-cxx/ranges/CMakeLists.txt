
add_executable(try-ranges)

target_sources(try-ranges
  PRIVATE
    main.cpp
    chain_view.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(try-ranges
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(try-ranges
  PRIVATE
    fmt::fmt
)

learn_cxx_common_compile_configs(try-ranges)
learn_cxx_clang_tidy(try-ranges)
learn_cxx_sanitizers(try-ranges)

get_target_property(try-ranges_FILES try-ranges SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${try-ranges_FILES})
