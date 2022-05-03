CPMAddPackage("gh:fmtlib/fmt#8.1.1")
CPMAddPackage("gh:kingsamchen/esl#9a1f02cbea4902ab110ce7e9457f226a2c2c3375")
CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  GIT_TAG v1.10.0
  OPTIONS "SPDLOG_FMT_EXTERNAL ON"
)

add_library(base)

target_sources(base
  PRIVATE
    exception.h
    subprocess.cpp
    subprocess.h
)

target_include_directories(base
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(base
  PUBLIC
    esl

  PRIVATE
    fmt
    spdlog
)

lumper_apply_common_compile_options(base)

if(LUMPER_USE_SANITIZER)
  lumper_apply_sanitizer(base)
endif()

target_precompile_headers(base
  PRIVATE "${LUMPER_PCH_HEADER}"
)
