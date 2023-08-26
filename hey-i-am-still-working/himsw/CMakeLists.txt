
add_executable(himsw WIN32)

target_sources(himsw
  PRIVATE
    main.cpp
    labor_monitor.cpp
    labor_monitor.h
    tray.cpp
    tray.h
    win_last_error.h

    resource.h
    himsw.rc
)

target_include_directories(himsw
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_compile_definitions(himsw
  PRIVATE
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

target_compile_options(himsw
  PRIVATE
    /utf-8
)

target_link_libraries(himsw
  PRIVATE
    fmt::fmt
    spdlog::spdlog
    tomlplusplus::tomlplusplus

    esl::esl
)

hey_i_am_still_working_apply_common_compile_options(himsw)

if(HEY_I_AM_STILL_WORKING_ENABLE_CLANG_TIDY)
  hey_i_am_still_working_apply_clang_tidy(himsw)
endif()

if(HEY_I_AM_STILL_WORKING_USE_MSVC_PARALLEL_BUILD)
  hey_i_am_still_working_apply_msvc_parallel_build(himsw)
endif()

if(HEY_I_AM_STILL_WORKING_USE_MSVC_STATIC_ANALYSIS)
  hey_i_am_still_working_apply_msvc_static_analysis(himsw
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()

get_target_property(himsw_FILES himsw SOURCES)
source_group("himsw" FILES ${himsw_FILES})

set_target_properties(himsw PROPERTIES
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

target_precompile_headers(himsw
  PRIVATE "${HEY_I_AM_STILL_WORKING_PCH_HEADER}"
)

