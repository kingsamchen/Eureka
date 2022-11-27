CPMAddPackage("gh:fmtlib/fmt#9.1.0")

add_executable(src-loc)

target_sources(src-loc
  PRIVATE
    main.cpp
    source_location.h
)

target_include_directories(src-loc
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(src-loc
  PUBLIC
    fmt
)

messagesourcelocation_apply_common_compile_options(src-loc)

# TODO: Edit at your will
if(MSVC)
  if(MESSAGESOURCELOCATION_USE_MSVC_PARALLEL_BUILD)
    messagesourcelocation_apply_msvc_parallel_build(src-loc)
  endif()
  if(MESSAGESOURCELOCATION_USE_MSVC_STATIC_ANALYSIS)
    messagesourcelocation_apply_msvc_static_analysis(src-loc
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
  get_target_property(src-loc_FILES src-loc SOURCES)
  source_group("src-loc" FILES ${src-loc_FILES})
else()
  if(MESSAGESOURCELOCATION_USE_SANITIZER)
    messagesourcelocation_apply_sanitizer(src-loc)
  endif()
endif()
