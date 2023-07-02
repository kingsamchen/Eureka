
option(TYPELIST_USE_SANITIZER "If enabled, activate address_sanitizer and ub_sanitizer" OFF)

if(TYPELIST_NOT_SUBPROJECT)
  message(STATUS "typelist compiler POSIX global conf is in active")

  # Force generating debugging symbols in Release build.
  # Also keep STL debugging symbols for clang builds.
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-limit-debug-info")
  endif()
endif()

message(STATUS "TYPELIST_USE_SANITIZER = ${TYPELIST_USE_SANITIZER}")

function(typelist_apply_common_compile_options TARGET)
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

function(typelist_apply_sanitizer TARGET)
  message(STATUS "Apply typelist sanitizer for ${TARGET}")

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