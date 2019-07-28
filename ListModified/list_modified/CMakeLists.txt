
declare_dep_module(KBase
  VERSION         master
  GIT_REPOSITORY  https://github.com/kingsamchen/KBase.git
  GIT_TAG         origin/master
)

set(list_modified_SRCS
  main.cpp
)

if(WIN32)
  list(APPEND list_modified_SRCS
  )
elseif(UNIX)
  list(APPEND list_modified_SRCS
  )
endif()

set(list_modified_HEADERS
)

if(WIN32)
  list(APPEND list_modified_HEADERS
  )
elseif(UNIX)
  list(APPEND list_modified_HEADERS
  )
endif()

set(list_modified_FILES ${list_modified_HEADERS} ${list_modified_SRCS})

source_group("list_modified" FILES ${list_modified_FILES})

add_executable(list_modified ${list_modified_FILES})

apply_common_compile_properties_to_target(list_modified)

if(MSVC)
  enable_msvc_static_analysis_for_target(list_modified)
endif()

# Uncomment if you use them.
#target_compile_definitions(list_modified
#)

target_link_libraries(list_modified
  PRIVATE kbase
)

target_include_directories(list_modified
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)
