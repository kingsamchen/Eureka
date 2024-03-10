
option(JSON_SERDE_USE_SANITIZER "If enabled, activate address_sanitizer and ub_sanitizer" OFF)

if(JSON_SERDE_NOT_SUBPROJECT)
  message(STATUS "json_serde compiler POSIX global conf is in active")

  # Force generating debugging symbols in Release build.
  # Also keep STL debugging symbols for clang builds.
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-limit-debug-info")
  endif()
endif()

message(STATUS "JSON_SERDE_USE_SANITIZER = ${JSON_SERDE_USE_SANITIZER}")

function(json_serde_apply_common_compile_options TARGET)
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
      -Werror
      -Wconversion
      -Wdouble-promotion
      -Wold-style-cast
      -Woverloaded-virtual
      -Wpointer-arith
      -Wshadow
      -Wsign-conversion
      -Wno-unused-function
      -Wno-error=deprecated
  )
endfunction()

function(json_serde_apply_sanitizer TARGET)
  message(STATUS "Apply json_serde sanitizer for ${TARGET}")

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
