
set(LEARN_CXX_USE_SANITIZERS "" CACHE STRING
  "Options are, case-insensitive: ASAN, UBSAN, TSAN. use ; to separate multiple sanitizer")
message(STATUS "LEARN_CXX_USE_SANITIZERS = ${LEARN_CXX_USE_SANITIZERS}")

set(LEARN_CXX_SANITIZERS_COMPILE_FLAGS "")
set(LEARN_CXX_SANITIZERS_LINK_FLAGS "")

if(LEARN_CXX_USE_SANITIZERS)
  if(MSVC)
    foreach(SANITIZER_ORIG IN LISTS LEARN_CXX_USE_SANITIZERS)
      string(TOUPPER "${SANITIZER_ORIG}" SANITIZER)

      if("${SANITIZER}" STREQUAL "ASAN")
        list(APPEND LEARN_CXX_SANITIZERS_COMPILE_FLAGS "/fsanitize=address")
        list(APPEND LEARN_CXX_SANITIZERS_LINK_FLAGS "/INCREMENTAL:NO")
      else()
        message(FATAL_ERROR "Unsupported sanitizer=${SANITIZER}")
      endif()
    endforeach()
  else()
    list(APPEND LEARN_CXX_SANITIZERS_COMPILE_FLAGS "-fno-omit-frame-pointer")

    foreach(SANITIZER_ORIG IN LISTS LEARN_CXX_USE_SANITIZERS)
      string(TOUPPER "${SANITIZER_ORIG}" SANITIZER)

      if("${SANITIZER}" STREQUAL "ASAN")
        list(APPEND LEARN_CXX_SANITIZERS_COMPILE_FLAGS "-fsanitize=address")
        list(APPEND LEARN_CXX_SANITIZERS_LINK_FLAGS "-fsanitize=address")
      elseif("${SANITIZER}" STREQUAL "UBSAN")
        list(APPEND LEARN_CXX_SANITIZERS_COMPILE_FLAGS "-fsanitize=undefined")
        list(APPEND LEARN_CXX_SANITIZERS_LINK_FLAGS "-fsanitize=undefined")
      elseif("${SANITIZER}" STREQUAL "TSAN")
        list(APPEND LEARN_CXX_SANITIZERS_COMPILE_FLAGS "-fsanitize=thread")
        list(APPEND LEARN_CXX_SANITIZERS_LINK_FLAGS "-fsanitize=thread")
      else()
        message(FATAL_ERROR "Unsupported sanitizer=${SANITIZER}")
      endif()
    endforeach()
  endif()
endif()

function(learn_cxx_sanitizers TARGET)
  if(NOT LEARN_CXX_USE_SANITIZERS)
    return()
  endif()

  target_compile_options(${TARGET}
    PRIVATE
      ${LEARN_CXX_SANITIZERS_COMPILE_FLAGS}
  )

  target_link_options(${TARGET}
    PRIVATE
      ${LEARN_CXX_SANITIZERS_LINK_FLAGS}
  )
endfunction()
