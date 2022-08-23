CPMAddPackage(
  NAME cppcodec
  URL "https://github.com/tplgy/cppcodec/archive/refs/tags/v0.2.tar.gz"
  OPTIONS "BUILD_TESTING OFF"
)

add_executable(topt)

target_sources(topt
  PRIVATE
    main.cpp
    shared_secret_key.cpp
    shared_secret_key.h
)

target_include_directories(topt
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(topt
  cppcodec
)

learn_topt_apply_common_compile_options(topt)

# TODO: Edit at your will
if(MSVC)
  if(LEARN_TOPT_USE_MSVC_PARALLEL_BUILD)
    message(STATUS "USE_MSVC_PARALLEL_BUILD is ON")
    learn_topt_apply_msvc_parallel_build(topt)
  endif()
  if(LEARN_TOPT_USE_MSVC_STATIC_ANALYSIS)
    message(STATUS "USE_MSVC_STATIC_ANALYSIS is ON")
    learn_topt_apply_msvc_static_analysis(topt
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
  get_target_property(topt_FILES topt SOURCES)
  source_group("topt" FILES ${topt_FILES})
else()
  if(LEARN_TOPT_USE_SANITIZER)
    message(STATUS "USE_SANITIZER is ON")
    learn_topt_apply_sanitizer(topt)
  endif()
endif()
