
add_executable(poly_no_virtual)

target_sources(poly_no_virtual
  PRIVATE
    main.cpp
    fptr_in_base.hpp
)

target_include_directories(poly_no_virtual
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(poly_no_virtual
  PRIVATE
    doctest::doctest
    fmt::fmt
)

learn_cxx_common_compile_configs(poly_no_virtual)
learn_cxx_sanitizers(poly_no_virtual)
