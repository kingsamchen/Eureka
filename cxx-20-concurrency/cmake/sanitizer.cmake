
set(CXX_20_CONCURRENCY_USE_SANITIZER "" CACHE STRING
  "Options are, case-insensitive: ASAN, UBSAN, TSAN. use ; to separate multiple sanitizer")
message(STATUS "CXX_20_CONCURRENCY_USE_SANITIZER = ${CXX_20_CONCURRENCY_USE_SANITIZER}")

set(CXX_20_CONCURRENCY_SANITIZER_COMPILE_FLAGS "")
set(CXX_20_CONCURRENCY_SANITIZER_LINK_FLAGS "")

if(CXX_20_CONCURRENCY_USE_SANITIZER)
  if(MSVC)
    foreach(SANITIZER_ORIG IN LISTS CXX_20_CONCURRENCY_USE_SANITIZER)
      string(TOUPPER "${SANITIZER_ORIG}" SANITIZER)

      if("${SANITIZER}" STREQUAL "ASAN")
        list(APPEND CXX_20_CONCURRENCY_SANITIZER_COMPILE_FLAGS "/fsanitize=address")
        list(APPEND CXX_20_CONCURRENCY_SANITIZER_LINK_FLAGS "/INCREMENTAL:NO")
      else()
        message(FATAL_ERROR "Unsupported sanitizer=${SANITIZER}")
      endif()
    endforeach()
  else()
    list(APPEND CXX_20_CONCURRENCY_SANITIZER_COMPILE_FLAGS "-fno-omit-frame-pointer")

    foreach(SANITIZER_ORIG IN LISTS CXX_20_CONCURRENCY_USE_SANITIZER)
      string(TOUPPER "${SANITIZER_ORIG}" SANITIZER)

      if("${SANITIZER}" STREQUAL "ASAN")
        list(APPEND CXX_20_CONCURRENCY_SANITIZER_COMPILE_FLAGS "-fsanitize=address")
        list(APPEND CXX_20_CONCURRENCY_SANITIZER_LINK_FLAGS "-fsanitize=address")
      elseif("${SANITIZER}" STREQUAL "UBSAN")
        list(APPEND CXX_20_CONCURRENCY_SANITIZER_COMPILE_FLAGS "-fsanitize=undefined")
        list(APPEND CXX_20_CONCURRENCY_SANITIZER_LINK_FLAGS "-fsanitize=undefined")
      elseif("${SANITIZER}" STREQUAL "TSAN")
        list(APPEND CXX_20_CONCURRENCY_SANITIZER_COMPILE_FLAGS "-fsanitize=thread")
        list(APPEND CXX_20_CONCURRENCY_SANITIZER_LINK_FLAGS "-fsanitize=thread")
      else()
        message(FATAL_ERROR "Unsupported sanitizer=${SANITIZER}")
      endif()
    endforeach()
  endif()
endif()

function(cxx_20_concurrency_apply_sanitizer TARGET)
  message(STATUS "Apply cxx_20_concurrency sanitizer for ${TARGET}")

  target_compile_options(${TARGET}
    PRIVATE
      ${CXX_20_CONCURRENCY_SANITIZER_COMPILE_FLAGS}
  )

  target_link_options(${TARGET}
    PRIVATE
      ${CXX_20_CONCURRENCY_SANITIZER_LINK_FLAGS}
  )
endfunction()
