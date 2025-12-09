
option(TRY_CONAN_CLANG_TIDY_ON_BUILD "Enable clang-tidy on build" OFF)
message(STATUS "TRY_CONAN_CLANG_TIDY_ON_BUILD = ${TRY_CONAN_CLANG_TIDY_ON_BUILD}")

if(TRY_CONAN_CLANG_TIDY_ON_BUILD)
  find_program(CLANG_TIDY_EXE
               NAMES clang-tidy
               DOC "Path to clang-tidy executable"
               REQUIRED)
  message(STATUS "Found clang-tidy = ${CLANG_TIDY_EXE}")
endif()

function(try_conan_clang_tidy_on_build TARGET)
  if(NOT TRY_CONAN_CLANG_TIDY_ON_BUILD)
    return()
  endif()

  if(MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties(${TARGET} PROPERTIES
      VS_GLOBAL_RunCodeAnalysis true
      VS_GLOBAL_EnableClangTidyCodeAnalysis true
    )
  else()
    set(CLANG_TIDY_COMMAND
      "${CLANG_TIDY_EXE}"
      "--quiet"
      "--extra-arg-before=--driver-mode=c++"
      "-p" "'${CMAKE_BINARY_DIR}'"
    )
    set_target_properties(${TARGET} PROPERTIES
      CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
    )
  endif()
endfunction()
