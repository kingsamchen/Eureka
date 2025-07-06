
add_executable(http_router)

target_sources(http_router
  PRIVATE
    main.cpp
    stringification.h

    tree.h
)

target_include_directories(http_router
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(http_router
  PRIVATE
    doctest::doctest
    esl::esl
    fmt::fmt
)

target_precompile_headers(http_router
  PRIVATE
    ${LEARN_CXX_PCH_HEADERS_BASE}
)

learn_cxx_common_compile_configs(http_router)
learn_cxx_sanitizers(http_router)
