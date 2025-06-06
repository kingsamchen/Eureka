
add_library(http_rest STATIC)

target_sources(http_rest
  PRIVATE
    server.cpp
    server.hpp
)

target_include_directories(http_rest
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(http_rest
  PRIVATE
    Boost::asio
)

learn_cxx_common_compile_configs(http_rest)
learn_cxx_sanitizers(http_rest)
