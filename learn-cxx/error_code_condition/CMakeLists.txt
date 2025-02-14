
add_executable(error_code_condition)

target_sources(error_code_condition
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(error_code_condition
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(error_code_condition
  PRIVATE
    fmt::fmt
)

learn_cxx_common_compile_configs(error_code_condition)
learn_cxx_sanitizers(error_code_condition)
learn_cxx_clang_tidy(error_code_condition)

get_target_property(error_code_condition_FILES error_code_condition SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${error_code_condition_FILES})
