
add_executable(use_chrono)

target_sources(use_chrono
  PRIVATE
    main.cpp
)

target_include_directories(use_chrono
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_compile_definitions(use_chrono
  PRIVATE
    FMT_USE_LOCAL_TIME=1
)

target_link_libraries(use_chrono
  PRIVATE
    doctest::doctest
    fmt::fmt
    date::date
    date::date-tz
)

learn_cxx_common_compile_configs(use_chrono)
learn_cxx_sanitizers(use_chrono)

get_target_property(use_chrono_FILES use_chrono SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${use_chrono_FILES})
