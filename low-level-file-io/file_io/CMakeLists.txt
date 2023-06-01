CPMAddPackage("gh:onqtam/doctest#v2.4.11")
CPMAddPackage("gh:kingsamchen/esl#caeca99503f5566782d7f60b44582c01761b3117")

add_executable(file_io)

target_sources(file_io
  PRIVATE
    main.cpp
    file_io.cpp
    file_io.h
)

target_include_directories(file_io
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(file_io
  PRIVATE
    doctest
    esl
)

low_level_file_io_apply_common_compile_options(file_io)

if(LOW_LEVEL_FILE_IO_ENABLE_CLANG_TIDY)
  low_level_file_io_apply_clang_tidy(file_io)
endif()

if(MSVC)
  if(LOW_LEVEL_FILE_IO_USE_MSVC_PARALLEL_BUILD)
    low_level_file_io_apply_msvc_parallel_build(file_io)
  endif()
  if(LOW_LEVEL_FILE_IO_USE_MSVC_STATIC_ANALYSIS)
    low_level_file_io_apply_msvc_static_analysis(file_io
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(LOW_LEVEL_FILE_IO_USE_SANITIZER)
    low_level_file_io_apply_sanitizer(file_io)
  endif()
endif()

get_target_property(file_io_FILES file_io SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${file_io_FILES})

add_test(NAME tests COMMAND file_io)
