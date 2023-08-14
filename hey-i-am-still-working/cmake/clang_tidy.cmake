
option(HEY_I_AM_STILL_WORKING_ENABLE_CLANG_TIDY "Enable clang-tidy on build" OFF)
message(STATUS "HEY_I_AM_STILL_WORKING_ENABLE_CLANG_TIDY = ${HEY_I_AM_STILL_WORKING_ENABLE_CLANG_TIDY}")

if(HEY_I_AM_STILL_WORKING_ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE
               NAMES clang-tidy
               DOC "Path to clang-tidy executable"
               REQUIRED)
  message(STATUS "Found clang-tidy = ${CLANG_TIDY_EXE}")
endif()

function(hey_i_am_still_working_apply_clang_tidy TARGET)
  message(STATUS "Apply hashids clang-tidy for ${TARGET}")

  if(MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties(${TARGET} PROPERTIES
      VS_GLOBAL_RunCodeAnalysis true
      VS_GLOBAL_EnableClangTidyCodeAnalysis true
    )
  else()
    set(CLANG_TIDY_COMMAND
      "${CLANG_TIDY_EXE}"
      "--quiet"
      "-p" "'${CMAKE_BINARY_DIR}'"
    )
    set_target_properties(${TARGET} PROPERTIES
      CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
    )
  endif()
endfunction()