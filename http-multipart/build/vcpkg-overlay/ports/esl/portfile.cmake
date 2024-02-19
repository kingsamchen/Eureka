vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO kingsamchen/esl
  REF f473cf84b74463689e365c00ba5a521a4526e858
  SHA512 8dc58395a32f4ec9261d37875a6f9bed8877ee9dcc35db278befe0c8f5a6d6a5aed6c615d3b6020dd699eb58103d1ecba763853bf0f3c7bcc1d1c0bea2e4fd3d
  HEAD_REF master
)

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}"
  OPTIONS
    -DESL_INSTALL_CMAKE_DIR=share/esl
    -DBUILD_TESTING=OFF
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup()

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")

file(REMOVE_RECURSE
  "${CURRENT_PACKAGES_DIR}/debug"
)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
