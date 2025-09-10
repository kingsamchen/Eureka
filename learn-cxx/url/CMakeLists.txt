
add_executable(try_url main.cpp)

target_link_libraries(try_url
  PRIVATE
    doctest::doctest
    fmt::fmt
    Boost::url
)

learn_cxx_common_compile_configs(try_url)
learn_cxx_sanitizers(try_url)
