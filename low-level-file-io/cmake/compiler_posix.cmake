
option(LOW_LEVEL_FILE_IO_USE_SANITIZER "If enabled, activate address_sanitizer and ub_sanitizer" OFF)

if(LOW_LEVEL_FILE_IO_NOT_SUBPROJECT)
  message(STATUS "low_level_file_io compiler POSIX global conf is in active")

  # Force generating debugging symbols in Release build.
  # Also keep STL debugging symbols for clang builds.
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-limit-debug-info")
  endif()
endif()

message(STATUS "LOW_LEVEL_FILE_IO_USE_SANITIZER = ${LOW_LEVEL_FILE_IO_USE_SANITIZER}")

function(low_level_file_io_apply_common_compile_options TARGET)
  target_compile_definitions(${TARGET}
    PUBLIC
      $<$<CONFIG:DEBUG>:
        _DEBUG
      >
  )

  target_compile_options(${TARGET}
    PRIVATE
      -Wall
      -Wextra
      -Wconversion
      -Wdouble-promotion
      -Wold-style-cast
      -Woverloaded-virtual
      -Wpointer-arith
      -Wshadow
      -Wsign-conversion
      -Wno-unused-function
  )
endfunction()

function(low_level_file_io_apply_sanitizer TARGET)
  message(STATUS "Apply low_level_file_io sanitizer for ${TARGET}")

  target_compile_options(${TARGET}
    PRIVATE
      -fno-omit-frame-pointer
      -fsanitize=address,undefined
  )

  target_link_libraries(${TARGET}
    PRIVATE
      -fsanitize=address,undefined
  )
endfunction()
