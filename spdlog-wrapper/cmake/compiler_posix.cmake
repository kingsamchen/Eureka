
include(${SPDLOG_WRAPPER_CMAKE_DIR}/utils.cmake)

if(SPDLOG_WRAPPER_NOT_SUBPROJECT)
  message(STATUS "spdlog_wrapper compiler POSIX global conf is in active")

  # Force generating debugging symbols in Release build.
  # Also keep STL debugging symbols for clang builds.
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-limit-debug-info")
  endif()
endif()

function(spdlog_wrapper_common_compile_configs TARGET)
  get_target_type(${TARGET} TARGET_TYPE)

  if(NOT TARGET_TYPE STREQUAL "INTERFACE_LIBRARY")
    target_compile_definitions(${TARGET}
      PRIVATE
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
  endif()
endfunction()
