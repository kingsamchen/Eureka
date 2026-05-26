
add_executable(feat)

target_sources(feat
  PRIVATE
    main.cpp

    space_ship_operator.cpp
)

target_include_directories(feat
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(feat
  PRIVATE
    doctest::doctest
    fmt::fmt
)

learn_cxx_common_compile_configs(feat)
learn_cxx_sanitizers(feat)
