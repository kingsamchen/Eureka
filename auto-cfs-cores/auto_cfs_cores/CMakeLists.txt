
declare_dep_module(KBase
  VERSION 6197a2af0
  GIT_REPOSITORY https://github.com/kingsamchen/KBase.git
  GIT_TAG origin/master
)

set(auto-cfs-cores_SRCS
  cfs_cores.cpp
)

set(auto-cfs-cores_HEADERS
  cfs_cores.h
)

set(auto-cfs-cores_FILES ${auto-cfs-cores_HEADERS} ${auto-cfs-cores_SRCS})

source_group("auto-cfs-cores" FILES ${auto-cfs-cores_FILES})

add_library(auto-cfs-cores ${auto-cfs-cores_FILES})

apply_auto_cfs_cores_compile_properties_to_target(auto-cfs-cores)

if(MSVC)
  enable_msvc_static_analysis_for_target(auto-cfs-cores)
endif()

set_target_properties(auto-cfs-cores PROPERTIES
  COTIRE_CXX_PREFIX_HEADER_INIT "${AUTO-CFS-CORES_PCH_HEADER}"
)

target_include_directories(auto-cfs-cores
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

# Uncomment if you use them.
#target_compile_definitions(auto-cfs-cores
#)

target_link_libraries(auto-cfs-cores
  PRIVATE kbase
)
