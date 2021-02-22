
add_library(uuid)

target_sources(uuid
  PRIVATE
    main.cpp

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(uuid
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(uuid
)

apply_uuid_compile_conf(uuid)

get_target_property(uuid_FILES uuid SOURCES)
source_group("uuid" FILES ${uuid_FILES})

if(MSVC AND UUID_ENABLE_CODE_ANALYSIS)
  enable_uuid_msvc_static_analysis_conf(uuid
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()

