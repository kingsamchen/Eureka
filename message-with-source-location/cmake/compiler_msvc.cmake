
option(MESSAGESOURCELOCATION_USE_MSVC_PARALLEL_BUILD "If enabled, build multiple files in parallel." ON)
option(MESSAGESOURCELOCATION_USE_MSVC_STATIC_ANALYSIS "If enabled, run MSVC built-in static analysis and generate appropriate warnings" ON)
option(MESSAGESOURCELOCATION_USE_WIN32_LEAN_AND_MEAN "If enabled, define WIN32_LEAN_AND_MEAN" ON)

if(MESSAGESOURCELOCATION_NOT_SUBPROJECT)
  message(STATUS "messagesourcelocation compiler MSVC global conf is in active")

  # Force generating debugging symbols in Release build.
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

message(STATUS "MESSAGESOURCELOCATION_USE_MSVC_PARALLEL_BUILD = ${MESSAGESOURCELOCATION_USE_MSVC_PARALLEL_BUILD}")
message(STATUS "MESSAGESOURCELOCATION_USE_MSVC_STATIC_ANALYSIS = $MESSAGESOURCELOCATION_USE_MSVC_STATIC_ANALYSIS}")
message(STATUS "MESSAGESOURCELOCATION_USE_WIN32_LEAN_AND_MEAN = ${MESSAGESOURCELOCATION_USE_WIN32_LEAN_AND_MEAN}")

function(messagesourcelocation_apply_common_compile_options TARGET)
  target_compile_definitions(${TARGET}
    PUBLIC
      _UNICODE
      UNICODE
      NOMINMAX

      $<$<CONFIG:DEBUG>:
        _DEBUG
      >

      $<$<BOOL:MESSAGESOURCELOCATION_USE_WIN32_LEAN_AND_MEAN>:WIN32_LEAN_AND_MEAN>
  )

  target_compile_options(${TARGET}
    PRIVATE
      /W4
      /wd4819 # source characters not in current code page.

      /Zc:inline            # Have the compiler eliminate unreferenced COMDAT functions and data before emitting the object file.
      /Zc:referenceBinding  # Disallow temporaries from binding to non-const lvalue references.
      /Zc:rvalueCast        # Enforce the standard rules for explicit type conversion.
      /Zc:implicitNoexcept  # Enable implicit noexcept specifications where required, such as destructors.
      /Zc:strictStrings     # Don't allow conversion from a string literal to mutable characters.
      /Zc:threadSafeInit    # Enable thread-safe function-local statics initialization.
      /Zc:throwingNew       # Assume operator new throws on failure.

      /permissive-  # Be mean, don't allow bad non-standard stuff (C++/CLI, __declspec, etc. are all left intact).
  )
endfunction()

function(messagesourcelocation_apply_msvc_parallel_build TARGET)
  message(STATUS "Apply messagesourcelocation msvc parallel build for ${TARGET}")

  target_compile_options(${TARGET}
    PRIVATE
      /MP
  )
endfunction()

# To explicitly suppress spcific warnings:
# messagesourcelocation_apply_msvc_static_analysis(foobar
#   WDL
#     /wd6011
# )
function(messagesourcelocation_apply_msvc_static_analysis TARGET)
  message(STATUS "Apply messagesourcelocation msvc static analysis for ${TARGET}")

  set(multiValueArgs WDL)
  cmake_parse_arguments(ARG "" "" "${multiValueArgs}" ${ARGN})

  target_compile_options(${TARGET}
    PRIVATE
      /analyze
      /analyze:WX-

      /wd6001 # Using uninitialized memory.
      /wd6011 # Dereferencing potentially NULL pointer.

      ${ARG_WDL}
  )
endfunction()
