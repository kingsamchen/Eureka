
add_executable(jthread_and_cancellation jthread_and_cancellation.cpp)

target_include_directories(jthread_and_cancellation
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(jthread_and_cancellation
  PRIVATE
    fmt::fmt
)

cxx_20_concurrency_apply_common_compile_options(jthread_and_cancellation)

if(CXX_20_CONCURRENCY_USE_CLANG_TIDY)
  cxx_20_concurrency_apply_clang_tidy(jthread_and_cancellation)
endif()

if(CXX_20_CONCURRENCY_USE_SANITIZER)
  cxx_20_concurrency_apply_sanitizer(jthread_and_cancellation)
endif()

if(MSVC)
  if(CXX_20_CONCURRENCY_USE_MSVC_PARALLEL_BUILD)
    cxx_20_concurrency_apply_msvc_parallel_build(jthread_and_cancellation)
  endif()
  if(CXX_20_CONCURRENCY_USE_MSVC_STATIC_ANALYSIS)
    cxx_20_concurrency_apply_msvc_static_analysis(jthread_and_cancellation
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
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

cxx_20_concurrency_apply_common_compile_options(use_latch)

if(CXX_20_CONCURRENCY_USE_CLANG_TIDY)
  cxx_20_concurrency_apply_clang_tidy(use_latch)
endif()

if(CXX_20_CONCURRENCY_USE_SANITIZER)
  cxx_20_concurrency_apply_sanitizer(use_latch)
endif()

if(MSVC)
  if(CXX_20_CONCURRENCY_USE_MSVC_PARALLEL_BUILD)
    cxx_20_concurrency_apply_msvc_parallel_build(use_latch)
  endif()
  if(CXX_20_CONCURRENCY_USE_MSVC_STATIC_ANALYSIS)
    cxx_20_concurrency_apply_msvc_static_analysis(use_latch
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
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

cxx_20_concurrency_apply_common_compile_options(use_sem)

if(CXX_20_CONCURRENCY_USE_CLANG_TIDY)
  cxx_20_concurrency_apply_clang_tidy(use_sem)
endif()

if(CXX_20_CONCURRENCY_USE_SANITIZER)
  cxx_20_concurrency_apply_sanitizer(use_sem)
endif()

if(MSVC)
  if(CXX_20_CONCURRENCY_USE_MSVC_PARALLEL_BUILD)
    cxx_20_concurrency_apply_msvc_parallel_build(use_sem)
  endif()
  if(CXX_20_CONCURRENCY_USE_MSVC_STATIC_ANALYSIS)
    cxx_20_concurrency_apply_msvc_static_analysis(use_sem
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
endif()

get_target_property(main_FILES jthread_and_cancellation SOURCES)
get_target_property(latch_FILES use_latch SOURCES)
get_target_property(sem_FILES use_sem SOURCES)
list(APPEND ${main_FILES} ${latch_FILES} ${sem_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${main_FILES})
