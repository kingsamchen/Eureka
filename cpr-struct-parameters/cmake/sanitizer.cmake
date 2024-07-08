
set(CPR_STRUCT_PARAMETERS_USE_SANITIZER "" CACHE STRING
  "Options are, case-insensitive: ASAN, UBSAN, TSAN. use ; to separate multiple sanitizer")
message(STATUS "CPR_STRUCT_PARAMETERS_USE_SANITIZER = ${CPR_STRUCT_PARAMETERS_USE_SANITIZER}")

set(CPR_STRUCT_PARAMETERS_SANITIZER_COMPILE_FLAGS "")
set(CPR_STRUCT_PARAMETERS_SANITIZER_LINK_FLAGS "")

if(CPR_STRUCT_PARAMETERS_USE_SANITIZER)
  if(MSVC)
    foreach(SANITIZER_ORIG IN LISTS CPR_STRUCT_PARAMETERS_USE_SANITIZER)
      string(TOUPPER "${SANITIZER_ORIG}" SANITIZER)

      if("${SANITIZER}" STREQUAL "ASAN")
        list(APPEND CPR_STRUCT_PARAMETERS_SANITIZER_COMPILE_FLAGS "/fsanitize=address")
        list(APPEND CPR_STRUCT_PARAMETERS_SANITIZER_LINK_FLAGS "/INCREMENTAL:NO")
      else()
        message(FATAL_ERROR "Unsupported sanitizer=${SANITIZER}")
      endif()
    endforeach()
  else()
    list(APPEND CPR_STRUCT_PARAMETERS_SANITIZER_COMPILE_FLAGS "-fno-omit-frame-pointer")

    foreach(SANITIZER_ORIG IN LISTS CPR_STRUCT_PARAMETERS_USE_SANITIZER)
      string(TOUPPER "${SANITIZER_ORIG}" SANITIZER)

      if("${SANITIZER}" STREQUAL "ASAN")
        list(APPEND CPR_STRUCT_PARAMETERS_SANITIZER_COMPILE_FLAGS "-fsanitize=address")
        list(APPEND CPR_STRUCT_PARAMETERS_SANITIZER_LINK_FLAGS "-fsanitize=address")
      elseif("${SANITIZER}" STREQUAL "UBSAN")
        list(APPEND CPR_STRUCT_PARAMETERS_SANITIZER_COMPILE_FLAGS "-fsanitize=undefined")
        list(APPEND CPR_STRUCT_PARAMETERS_SANITIZER_LINK_FLAGS "-fsanitize=undefined")
      elseif("${SANITIZER}" STREQUAL "TSAN")
        list(APPEND CPR_STRUCT_PARAMETERS_SANITIZER_COMPILE_FLAGS "-fsanitize=thread")
        list(APPEND CPR_STRUCT_PARAMETERS_SANITIZER_LINK_FLAGS "-fsanitize=thread")
      else()
        message(FATAL_ERROR "Unsupported sanitizer=${SANITIZER}")
      endif()
    endforeach()
  endif()
endif()

function(cpr_struct_parameters_apply_sanitizer TARGET)
  message(STATUS "Apply cpr_struct_parameters sanitizer for ${TARGET}")

  target_compile_options(${TARGET}
    PRIVATE
      ${CPR_STRUCT_PARAMETERS_SANITIZER_COMPILE_FLAGS}
  )

  target_link_options(${TARGET}
    PRIVATE
      ${CPR_STRUCT_PARAMETERS_SANITIZER_LINK_FLAGS}
  )
endfunction()
