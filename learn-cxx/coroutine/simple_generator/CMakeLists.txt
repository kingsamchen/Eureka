
add_executable(coro_generator)

target_sources(coro_generator
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(coro_generator
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(coro_generator
)

learn_cxx_common_compile_configs(coro_generator)

if(LEARN_CXX_USE_CLANG_TIDY)
  learn_cxx_clang_tidy(coro_generator)
endif()

if(LEARN_CXX_USE_SANITIZERS)
  learn_cxx_sanitizers(coro_generator)
endif()

get_target_property(coro_generator_FILES coro_generator SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${coro_generator_FILES})
