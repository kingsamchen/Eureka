vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO kingsamchen/esl
  REF 0930ae339850624b1d76dd4d16d8d620b3ce78a5
  SHA512 f99072c5bee89fa6801e61205832fdac5fbd0b39689e4b88b317e00b12fec0931c815a72eba2213ad31e486cd92c60e530896042fba72976db4c15d67e4ed43c
  HEAD_REF dev-install-as-package
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
