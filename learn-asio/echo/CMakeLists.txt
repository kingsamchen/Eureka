
declare_dep_module(KBase
  VERSION         latest
  GIT_REPOSITORY  https://github.com/kingsamchen/KBase.git
  GIT_TAG         origin/master
)

add_executable(echo_srv echo_server.cpp)
apply_learn_asio_compile_properties_to_target(echo_srv)
target_link_libraries(echo_srv PRIVATE kbase asio)
