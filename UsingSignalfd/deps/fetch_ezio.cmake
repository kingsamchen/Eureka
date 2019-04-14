
set(EZIO_SOURCE_NAME ezio)
set(EZIO_SOURCE_VER 8dd166a4073ea41491a9bef0096fd92f7a98e7cf)

# Content name is used by the module and in the lowercase.
string(TOLOWER "${EZIO_SOURCE_NAME}-${EZIO_SOURCE_VER}" EZIO_CONTENT_NAME)

# Source dir is where we put the source code of the dependency.
set(EZIO_SOURCE_DIR "${DEPS_SOURCE_DIR}/${EZIO_CONTENT_NAME}-src")

FetchContent_Declare(
  ${EZIO_CONTENT_NAME}
  GIT_REPOSITORY  https://github.com/kingsamchen/ezio.git
  GIT_TAG         ${EZIO_SOURCE_VER}
  GIT_SHALLOW     TRUE
  SOURCE_DIR      ${EZIO_SOURCE_DIR}
)

FetchContent_GetProperties(${EZIO_CONTENT_NAME})

if(NOT ${EZIO_CONTENT_NAME}_POPULATED)
  message(STATUS "Fetching dep: ${EZIO_CONTENT_NAME}")

  if(EXISTS "${EZIO_SOURCE_DIR}/CMakeLists.txt")
    message(STATUS "${EZIO_CONTENT_NAME} source dir is already ready; skip downloading.")
    set(FETCHCONTENT_SOURCE_DIR_EZIO ${EZIO_SOURCE_DIR})
  endif()

  FetchContent_Populate(${EZIO_CONTENT_NAME})

  # Set two module-defined variables.
  set(EZIO_CONTENT_SOURCE_DIR_VAR "${${EZIO_CONTENT_NAME}_SOURCE_DIR}")
  set(EZIO_CONTENT_BINARY_DIR_VAR "${${EZIO_CONTENT_NAME}_BINARY_DIR}")

  message(STATUS "${EZIO_CONTENT_NAME} source dir: ${EZIO_CONTENT_SOURCE_DIR_VAR}")
  message(STATUS "${EZIO_CONTENT_NAME} binary dir: ${EZIO_CONTENT_BINARY_DIR_VAR}")

  add_subdirectory(${EZIO_CONTENT_SOURCE_DIR_VAR} ${EZIO_CONTENT_BINARY_DIR_VAR})
endif()
