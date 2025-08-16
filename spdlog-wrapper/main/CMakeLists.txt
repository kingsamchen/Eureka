
add_executable(main)

target_sources(main
  PRIVATE
    main.cpp
    logging.hpp
    logging.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(main
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(main
  PRIVATE
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    gflags::gflags
)

spdlog_wrapper_common_compile_configs(main)

spdlog_wrapper_clang_tidy_on_build(main)

spdlog_wrapper_use_sanitizers(main)

get_target_property(main_FILES main SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${main_FILES})
