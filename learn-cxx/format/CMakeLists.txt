
add_executable(use_fmt)

target_sources(use_fmt
  PRIVATE
    main.cpp
)

target_compile_features(use_fmt
  PRIVATE
    cxx_std_23
)

target_include_directories(use_fmt
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(use_fmt
  PRIVATE
    doctest::doctest
    fmt::fmt
)

learn_cxx_common_compile_configs(use_fmt)
learn_cxx_sanitizers(use_fmt)
