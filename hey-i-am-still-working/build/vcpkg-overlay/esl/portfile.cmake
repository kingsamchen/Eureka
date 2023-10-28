vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO kingsamchen/esl
  REF 9014513de157fc3bb739d687d5cffebe761075e1
  SHA512 1d8b27b071e178ebc9d01144ec97ff1cf28d7562c191253c07dc5bc889e9a8e531be9a7c71811ef7ede30a7c0f7d648feb45cc384f933e43d3fcede0360fe918
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
