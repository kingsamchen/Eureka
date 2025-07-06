
add_executable(simple_rest_server)

target_sources(simple_rest_server
  PRIVATE
    main.cpp
)

target_include_directories(simple_rest_server
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(simple_rest_server
  PRIVATE
    Boost::asio
    Boost::beast
    fmt::fmt
    gflags::gflags
)

target_precompile_headers(simple_rest_server
  PRIVATE
    ${LEARN_CXX_PCH_HEADERS_BASE}
    <boost/asio.hpp>
)

learn_cxx_common_compile_configs(simple_rest_server)
learn_cxx_sanitizers(simple_rest_server)

set_target_properties(simple_rest_server PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
