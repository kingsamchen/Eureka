
CPMAddPackage(
  NAME KBase
  GITHUB_REPOSITORY kingsamchen/KBase
  VERSION 0.1.6
)

add_executable(conn_pool)

target_sources(conn_pool
  PRIVATE
    main.cpp
    pool.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(conn_pool
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(conn_pool
  PRIVATE kbase
)

apply_connpool_compile_conf(conn_pool)

get_target_property(conn_pool_FILES conn_pool SOURCES)
source_group("conn_pool" FILES ${conn_pool_FILES})

if(MSVC AND CONNPOOL_ENABLE_CODE_ANALYSIS)
  enable_connpool_msvc_static_analysis_conf(conn_pool
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()

