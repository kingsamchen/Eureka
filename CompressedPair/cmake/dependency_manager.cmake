
include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${DEPS_DEPLOY_DIR})

function(declare_dep_module MODULE_NAME)
  set(options "")

  set(oneValueArgs
    VERSION
    GIT_REPOSITORY
    GIT_TAG
  )

  set(multiValueArgs "")

  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # Content name is used by the module and in the lowercase.
  string(TOLOWER "${MODULE_NAME}-${ARG_VERSION}" MODULE_CONTENT_NAME)
  string(TOUPPER "${MODULE_CONTENT_NAME}" UC_MODULE_CONTENT_NAME)

  # Source dir is where we put the source code of the dependency.
  # `DEPS_SOURCE_DIR` is defined by the root project.
  set(MODULE_SOURCE_DIR "${DEPS_SOURCE_DIR}/${MODULE_CONTENT_NAME}-src")

  FetchContent_Declare(
    ${MODULE_CONTENT_NAME}
    GIT_REPOSITORY  ${ARG_GIT_REPOSITORY}
    GIT_TAG         ${ARG_GIT_TAG}
    GIT_SHALLOW     TRUE
    SOURCE_DIR      ${MODULE_SOURCE_DIR}
  )

  FetchContent_GetProperties(${MODULE_CONTENT_NAME})

  if(NOT ${MODULE_CONTENT_NAME}_POPULATED)
    message(STATUS "Fetching dep: ${MODULE_CONTENT_NAME}")

    if(EXISTS "${MODULE_SOURCE_DIR}/CMakeLists.txt")
      set("FETCHCONTENT_SOURCE_DIR_${UC_MODULE_CONTENT_NAME}" ${MODULE_SOURCE_DIR})
      message(STATUS "${MODULE_CONTENT_NAME} source dir is found at: ${FETCHCONTENT_SOURCE_DIR_${UC_MODULE_CONTENT_NAME}}; skip fetching.")
    endif()

    FetchContent_Populate(${MODULE_CONTENT_NAME})

    # Set two module-defined variables.
    set("${MODULE_CONTENT_NAME}_CONTENT_SOURCE_DIR_VAR" "${${MODULE_CONTENT_NAME}_SOURCE_DIR}")
    set("${MODULE_CONTENT_NAME}_CONTENT_BINARY_DIR_VAR" "${${MODULE_CONTENT_NAME}_BINARY_DIR}")

    message(STATUS "${MODULE_CONTENT_NAME} source dir: ${${MODULE_CONTENT_NAME}_CONTENT_SOURCE_DIR_VAR}")
    message(STATUS "${MODULE_CONTENT_NAME} binary dir: ${${MODULE_CONTENT_NAME}_CONTENT_BINARY_DIR_VAR}")

    add_subdirectory("${${MODULE_CONTENT_NAME}_CONTENT_SOURCE_DIR_VAR}" "${${MODULE_CONTENT_NAME}_CONTENT_BINARY_DIR_VAR}")
  endif()

endfunction(declare_dep_module)
