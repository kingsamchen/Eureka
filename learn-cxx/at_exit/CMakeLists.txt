
add_executable(at_exit)

target_compile_features(at_exit
  PRIVATE
    cxx_std_23
)

target_sources(at_exit
  PRIVATE
    main.cpp

    at_exit.hpp
    at_exit.cpp
)

target_include_directories(at_exit
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(at_exit
  PRIVATE
    doctest::doctest
    fmt::fmt
)

learn_cxx_common_compile_configs(at_exit)
learn_cxx_sanitizers(at_exit)
