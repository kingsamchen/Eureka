
add_executable(http_router)

target_sources(http_router
  PRIVATE
    main.cpp
)

target_include_directories(http_router
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(http_router
  PRIVATE
    doctest::doctest
)

learn_cxx_apply_common_compile_options(http_router)

if(LEARN_CXX_USE_SANITIZER)
  learn_cxx_apply_sanitizer(http_router)
endif()
