
# Force generating debugging symbols in Release build.
# Also keep STL debugging symbols for clang builds.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-limit-debug-info")
endif()

function(apply_crow_app_compile_conf TARGET)
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
      #-Werror
      -Wno-unused-parameter
      #-Wold-style-cast
      -Woverloaded-virtual
      -Wpointer-arith
      -Wshadow
  )

  set_target_properties(${TARGET} PROPERTIES
    LINK_FLAGS "-rdynamic" # Currently required by backtrace_symbols(2)
  )

endfunction()
