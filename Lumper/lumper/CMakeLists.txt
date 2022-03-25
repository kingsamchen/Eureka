CPMAddPackage("gh:fmtlib/fmt#8.1.1")
CPMAddPackage("gh:kingsamchen/esl#9a1f02cbea4902ab110ce7e9457f226a2c2c3375")
CPMAddPackage("gh:p-ranav/argparse#v2.2")

add_executable(lumper)

target_sources(lumper
  PRIVATE
    main.cpp
    cli.cpp
    cli.h
)

target_include_directories(lumper
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(lumper
  PRIVATE
    argparse
    esl
    fmt
)

lumper_apply_common_compile_options(lumper)

if(LUMPER_USE_SANITIZER)
  lumper_apply_sanitizer(lumper)
endif()

target_precompile_headers(lumper
  PRIVATE "${LUMPER_PCH_HEADER}"
)
