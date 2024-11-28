
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

learn_cxx_apply_common_compile_options(simple_rest_server)

if(LEARN_CXX_USE_SANITIZER)
  learn_cxx_apply_sanitizer(simple_rest_server)
endif()
