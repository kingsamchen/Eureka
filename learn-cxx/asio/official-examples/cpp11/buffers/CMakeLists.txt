
add_executable(cpp11_ref_counted)

target_sources(cpp11_ref_counted
  PRIVATE
    reference_counted.cpp
)

target_link_libraries(cpp11_ref_counted
  PRIVATE
    Boost::asio
)
