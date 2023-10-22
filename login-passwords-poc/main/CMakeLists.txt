
add_executable(main)

target_sources(main
  PRIVATE
    auth_passwords.cpp
    auth_passwords.h
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(main
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(main
  PRIVATE
    doctest::doctest
    PkgConfig::botan-3
)

login_passwords_poc_apply_common_compile_options(main)

if(LOGIN_PASSWORDS_POC_ENABLE_CLANG_TIDY)
  login_passwords_poc_apply_clang_tidy(main)
endif()

# TODO: Edit at your will
if(MSVC)
  if(LOGIN_PASSWORDS_POC_USE_MSVC_PARALLEL_BUILD)
    login_passwords_poc_apply_msvc_parallel_build(main)
  endif()
  if(LOGIN_PASSWORDS_POC_USE_MSVC_STATIC_ANALYSIS)
    login_passwords_poc_apply_msvc_static_analysis(main
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(LOGIN_PASSWORDS_POC_USE_SANITIZER)
    login_passwords_poc_apply_sanitizer(main)
  endif()
endif()

get_target_property(main_FILES main SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${main_FILES})

target_precompile_headers(main
  PRIVATE "${LOGIN_PASSWORDS_POC_PCH_HEADER}"
)