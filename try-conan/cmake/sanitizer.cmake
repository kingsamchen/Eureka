
set(TRY_CONAN_USE_SANITIZERS "" CACHE STRING
  "Options are, case-insensitive: ASAN, UBSAN, TSAN. use ; to separate multiple sanitizer")
message(STATUS "TRY_CONAN_USE_SANITIZERS = ${TRY_CONAN_USE_SANITIZERS}")

set(TRY_CONAN_SANITIZER_COMPILE_FLAGS "")
set(TRY_CONAN_SANITIZER_LINK_FLAGS "")

if(TRY_CONAN_USE_SANITIZERS)
  if(MSVC)
    # See https://learn.microsoft.com/en-us/cpp/sanitizers/asan-known-issues?#incompatible-options
    string(REGEX REPLACE "/RTC[1scu]" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REGEX REPLACE "/RTC[1scu]" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    list(APPEND TRY_CONAN_SANITIZER_LINK_FLAGS "/INCREMENTAL:NO")

    foreach(SANITIZER_ORIG IN LISTS TRY_CONAN_USE_SANITIZERS)
      string(TOUPPER "${SANITIZER_ORIG}" SANITIZER)

      if("${SANITIZER}" STREQUAL "ASAN")
        list(APPEND TRY_CONAN_SANITIZER_COMPILE_FLAGS "/fsanitize=address")
      else()
        message(FATAL_ERROR "Unsupported sanitizer=${SANITIZER}")
      endif()
    endforeach()
  else()
    list(APPEND TRY_CONAN_SANITIZER_COMPILE_FLAGS "-fno-omit-frame-pointer")

    foreach(SANITIZER_ORIG IN LISTS TRY_CONAN_USE_SANITIZERS)
      string(TOUPPER "${SANITIZER_ORIG}" SANITIZER)

      if("${SANITIZER}" STREQUAL "ASAN")
        list(APPEND TRY_CONAN_SANITIZER_COMPILE_FLAGS "-fsanitize=address")
        list(APPEND TRY_CONAN_SANITIZER_LINK_FLAGS "-fsanitize=address")
      elseif("${SANITIZER}" STREQUAL "UBSAN")
        list(APPEND TRY_CONAN_SANITIZER_COMPILE_FLAGS "-fsanitize=undefined")
        list(APPEND TRY_CONAN_SANITIZER_LINK_FLAGS "-fsanitize=undefined")
      elseif("${SANITIZER}" STREQUAL "TSAN")
        list(APPEND TRY_CONAN_SANITIZER_COMPILE_FLAGS "-fsanitize=thread")
        list(APPEND TRY_CONAN_SANITIZER_LINK_FLAGS "-fsanitize=thread")
      else()
        message(FATAL_ERROR "Unsupported sanitizer=${SANITIZER}")
      endif()
    endforeach()
  endif()
endif()

function(try_conan_use_sanitizers TARGET)
  if(NOT TRY_CONAN_USE_SANITIZERS)
    return()
  endif()

  target_compile_options(${TARGET}
    PRIVATE
      ${TRY_CONAN_SANITIZER_COMPILE_FLAGS}
  )

  target_link_options(${TARGET}
    PRIVATE
      ${TRY_CONAN_SANITIZER_LINK_FLAGS}
  )
endfunction()
