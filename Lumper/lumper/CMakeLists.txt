CPMAddPackage("gh:kingsamchen/esl#9a1f02cbea4902ab110ce7e9457f226a2c2c3375")

add_executable(lumper)

target_sources(lumper
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(lumper
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(lumper
)

lumper_apply_common_compile_options(lumper)

if(LUMPER_USE_SANITIZER)
  lumper_apply_sanitizer(lumper)
endif()

target_precompile_headers(lumper
  PRIVATE "${LUMPER_PCH_HEADER}"
)
