CPMAddPackage("gh:kingsamchen/esl#9a1f02cbea4902ab110ce7e9457f226a2c2c3375")

add_library(base)

target_sources(base
  PRIVATE
    subprocess.cpp
    subprocess.h
)

target_include_directories(base
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(base
  PRIVATE
    esl
)

lumper_apply_common_compile_options(base)

if(LUMPER_USE_SANITIZER)
  lumper_apply_sanitizer(base)
endif()

target_precompile_headers(base
  PRIVATE "${LUMPER_PCH_HEADER}"
)
