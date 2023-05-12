
option(HTTP_MULTIPART_USE_SANITIZER "If enabled, activate address_sanitizer and ub_sanitizer" OFF)

if(HTTP_MULTIPART_NOT_SUBPROJECT)
  message(STATUS "http_multipart compiler POSIX global conf is in active")

  # Force generating debugging symbols in Release build.
  # Also keep STL debugging symbols for clang builds.
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-limit-debug-info")
  endif()
endif()

message(STATUS "HTTP_MULTIPART_USE_SANITIZER = ${HTTP_MULTIPART_USE_SANITIZER}")

function(http_multipart_apply_common_compile_options TARGET)
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
      -Wno-deprecated
      -Wno-deprecated-declarations
      -Wno-sign-compare
      -Wno-unused
      -Wno-unused-parameter
      -Wold-style-cast
      -Woverloaded-virtual
      -Wpointer-arith
      -Wshadow
      -Wunused-label
      -Wunused-result
  )
endfunction()

function(http_multipart_apply_sanitizer TARGET)
  message(STATUS "Apply http_multipart sanitizer for ${TARGET}")

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
