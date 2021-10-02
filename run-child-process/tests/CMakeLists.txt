CPMAddPackage("gh:onqtam/doctest#2.4.6")

add_executable(rcp_app)

target_sources(rcp_app
  PRIVATE
    rcp_app.cpp
)

apply_run_child_process_compile_conf(rcp_app)

get_target_property(rcp_app_FILES rcp_app SOURCES)
source_group("rcp_app" FILES ${rcp_app_FILES})

# ----------------------------------------------------

add_executable(test_rcp)

target_sources(test_rcp
  PRIVATE
    rcp_test.cpp
)

apply_run_child_process_compile_conf(test_rcp)

target_link_libraries(test_rcp
  PRIVATE
    rcp
    doctest::doctest
)

get_target_property(test_FILES test_rcp SOURCES)
source_group("test_rcp" FILES ${test_FILES})

add_test(NAME tests COMMAND test_rcp)
