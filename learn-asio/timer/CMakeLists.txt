
set(sync_timer_SRCS
  sync_timer.cpp
)

set(sync_timer_HEADERS
)

set(sync_timer_FILES ${sync_timer_HEADERS} ${sync_timer_SRCS})

source_group("src" FILES ${sync_timer_FILES})

add_executable(sync_timer ${sync_timer_FILES})

apply_learn_asio_compile_properties_to_target(sync_timer)

target_include_directories(sync_timer
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(sync_timer
  PRIVATE asio
)

# async-timer

set(async_timer_SRCS
  async_timer.cpp
)

set(async_timer_HEADERS
)

set(async_timer_FILES ${async_timer_HEADERS} ${async_timer_SRCS})

source_group("src" FILES ${async_timer_FILES})

add_executable(async_timer ${async_timer_FILES})

apply_learn_asio_compile_properties_to_target(async_timer)

target_include_directories(async_timer
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(async_timer
  PRIVATE asio
)

# timer-handler

add_executable(timer_handler timer_handler.cpp)
apply_learn_asio_compile_properties_to_target(timer_handler)
target_link_libraries(timer_handler PRIVATE asio)

# mt-sync-handler

add_executable(mt_sync mt_sync_handler.cpp)
apply_learn_asio_compile_properties_to_target(mt_sync)
target_link_libraries(mt_sync PRIVATE asio)
