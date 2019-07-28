
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "BUILD_TYPE = " ${BUILD_TYPE})

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")

function(apply_common_compile_properties_to_target TARGET)
  target_compile_options(${TARGET}
    PRIVATE
      -g
      -Wall
      -Wextra
      -Werror
      -Wno-unused-parameter
      -Wold-style-cast
      -Woverloaded-virtual
      -Wpointer-arith
      -Wshadow

      $<$<STREQUAL:${CMAKE_CXX_COMPILER_ID},"Clang">:-fno-limit-debug-info>
  )
endfunction(apply_common_compile_properties_to_target)
