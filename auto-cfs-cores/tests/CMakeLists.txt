
declare_dep_module(Catch2
  VERSION         2.10.2
  GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
  GIT_TAG         v2.10.2
)

set(tests_SRCS
  main.cpp
)

source_group("tests" FILES ${tests_SRCS})

add_executable(tests ${tests_SRCS})

apply_auto_cfs_cores_compile_properties_to_target(tests)

target_link_libraries(tests
  PRIVATE Catch2 auto-cfs-cores
)
