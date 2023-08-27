vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO kingsamchen/uuidxx
  REF cf07877cdfab8fef712d069c98acd5dde5ed008d
  SHA512 a753184b1c79acf3bdc8eb32fd3fd1931fb1cad8f7a1de197098601d7d443ff89b2580ba230588ef2666426d848ba871e5c6f752afd18a11b3b28e25a514f4bf
  HEAD_REF dev-install-as-package
)

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}"
  OPTIONS
    -DUUIDXX_INSTALL_CMAKE_DIR=share/uuidxx
    -DBUILD_TESTING=OFF
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup()

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")

file(REMOVE_RECURSE
  "${CURRENT_PACKAGES_DIR}/debug/include"
)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
