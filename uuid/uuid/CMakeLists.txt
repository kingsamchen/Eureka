
add_executable(uuid)

target_sources(uuid
  PRIVATE
    main.cpp
    uuid.h
    uuid.cpp
    mac_address.cpp
    md5.cpp
    md5.h
    endian_utils.h

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

set_target_properties(uuid PROPERTIES
  # Use clangtidy
  VS_GLOBAL_EnableClangTidyCodeAnalysis true
  #VS_GLOBAL_ClangTidyChecks -checks=-*,modernize-*,-modernize-use-trailing-return-type
)

if(MSVC AND UUID_ENABLE_CODE_ANALYSIS)
  enable_uuid_msvc_static_analysis_conf(uuid
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()
