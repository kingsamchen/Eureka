
add_executable(jthread_and_cancellation jthread_and_cancellation.cpp)

target_include_directories(jthread_and_cancellation
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(jthread_and_cancellation
  PRIVATE
    fmt::fmt
)

learn_cxx_common_compile_configs(jthread_and_cancellation)

if(LEARN_CXX_USE_CLANG_TIDY)
  learn_cxx_clang_tidy(jthread_and_cancellation)
endif()

if(LEARN_CXX_USE_SANITIZERS)
  learn_cxx_sanitizers(jthread_and_cancellation)
endif()

#
# latch
#

add_executable(use_latch latch.cpp)

target_include_directories(use_latch
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(use_latch
  PRIVATE
    fmt::fmt
)

learn_cxx_common_compile_configs(use_latch)

if(LEARN_CXX_USE_CLANG_TIDY)
  learn_cxx_clang_tidy(use_latch)
endif()

if(LEARN_CXX_USE_SANITIZERS)
  learn_cxx_sanitizers(use_latch)
endif()

#
# use_sem
#

add_executable(use_sem semaphore.cpp)

target_include_directories(use_sem
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(use_sem
  PRIVATE
    fmt::fmt
)

learn_cxx_common_compile_configs(use_sem)

if(LEARN_CXX_USE_CLANG_TIDY)
  learn_cxx_clang_tidy(use_sem)
endif()

if(LEARN_CXX_USE_SANITIZERS)
  learn_cxx_sanitizers(use_sem)
endif()

get_target_property(concurrency_FILES jthread_and_cancellation SOURCES)
get_target_property(latch_FILES use_latch SOURCES)
get_target_property(sem_FILES use_sem SOURCES)
list(APPEND ${concurrency_FILES} ${latch_FILES} ${sem_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${concurrency_FILES})
