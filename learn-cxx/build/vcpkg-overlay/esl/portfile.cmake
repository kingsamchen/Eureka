vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO kingsamchen/esl
  REF d80c2271ffdd7e26fc85c06be2d67756514ec81d
  SHA512 19b90f4dd443e1ecfadee6ba3a15f048b08a49ddf2ab2a6f94c25e854e778b0d3341e273a4598e1fd79a1c26c62129359e5990bc8429c4d83a91a3fbeb2f62b7
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
