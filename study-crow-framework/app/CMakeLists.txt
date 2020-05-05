
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system)

add_library(crow INTERFACE)

target_include_directories(crow
    INTERFACE ${CMAKE_SOURCE_DIR}/crow/include
)

add_executable(crow_app main.cpp)

target_include_directories(crow_app
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/
  PRIVATE ${Boost_INCLUDE_DIR}
)

target_link_libraries(crow_app
  PRIVATE crow Boost::system pthread
)

apply_crow_app_compile_conf(crow_app)
