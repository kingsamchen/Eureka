
option(CXX_20_CONCURRENCY_USE_MSVC_PARALLEL_BUILD "If enabled, build multiple files in parallel." ON)
option(CXX_20_CONCURRENCY_USE_MSVC_STATIC_ANALYSIS "If enabled, run MSVC built-in static analysis and generate appropriate warnings" OFF)
option(CXX_20_CONCURRENCY_USE_WIN32_LEAN_AND_MEAN "If enabled, define WIN32_LEAN_AND_MEAN" ON)

if(CXX_20_CONCURRENCY_NOT_SUBPROJECT)
  message(STATUS "cxx_20_concurrency compiler MSVC global conf is in active")

  # Force generating debugging symbols in Release build.
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

message(STATUS "CXX_20_CONCURRENCY_USE_MSVC_PARALLEL_BUILD = ${CXX_20_CONCURRENCY_USE_MSVC_PARALLEL_BUILD}")
message(STATUS "CXX_20_CONCURRENCY_USE_MSVC_STATIC_ANALYSIS = ${CXX_20_CONCURRENCY_USE_MSVC_STATIC_ANALYSIS}")
message(STATUS "CXX_20_CONCURRENCY_USE_WIN32_LEAN_AND_MEAN = ${CXX_20_CONCURRENCY_USE_WIN32_LEAN_AND_MEAN}")

function(cxx_20_concurrency_apply_common_compile_options TARGET)
  target_compile_definitions(${TARGET}
    PUBLIC
      _UNICODE
      UNICODE
      NOMINMAX

      $<$<CONFIG:DEBUG>:
        _DEBUG
      >

      $<$<BOOL:CXX_20_CONCURRENCY_USE_WIN32_LEAN_AND_MEAN>:WIN32_LEAN_AND_MEAN>
  )

  target_compile_options(${TARGET}
    PRIVATE
      /W4
      /wd4819 # source characters not in current code page.

      /Zc:inline            # Have the compiler eliminate unreferenced COMDAT functions and data before emitting the object file.
      /Zc:rvalueCast        # Enforce the standard rules for explicit type conversion.
      /Zc:strictStrings     # Don't allow conversion from a string literal to mutable characters.
      /Zc:threadSafeInit    # Enable thread-safe function-local statics initialization.

      /permissive-  # Be mean, don't allow bad non-standard stuff (C++/CLI, __declspec, etc. are all left intact).
  )
endfunction()

function(cxx_20_concurrency_apply_msvc_parallel_build TARGET)
  message(STATUS "Apply cxx_20_concurrency msvc parallel build for ${TARGET}")

  target_compile_options(${TARGET}
    PRIVATE
      /MP
  )
endfunction()

# To explicitly suppress spcific warnings:
# cxx_20_concurrency_apply_msvc_static_analysis(foobar
#   WDL
#     /wd6011
# )
function(cxx_20_concurrency_apply_msvc_static_analysis TARGET)
  message(STATUS "Apply cxx_20_concurrency msvc static analysis for ${TARGET}")

  set(multiValueArgs WDL)
  cmake_parse_arguments(ARG "" "" "${multiValueArgs}" ${ARGN})

  target_compile_options(${TARGET}
    PRIVATE
      /analyze

      /wd6001 # Using uninitialized memory.
      /wd6011 # Dereferencing potentially NULL pointer.

      ${ARG_WDL}
  )

  if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties(${TARGET} PROPERTIES
      VS_GLOBAL_EnableMicrosoftCodeAnalysis true
      VS_GLOBAL_RunCodeAnalysis true
    )
  endif()
endfunction()
