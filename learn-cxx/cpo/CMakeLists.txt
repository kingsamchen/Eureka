
add_executable(cpo)

target_sources(cpo
  PRIVATE
    main.cpp
    cpo.h
)

target_include_directories(cpo
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(cpo
  PRIVATE
    doctest::doctest
    fmt::fmt
)

learn_cxx_common_compile_configs(cpo)
learn_cxx_sanitizers(cpo)
