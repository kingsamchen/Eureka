#!/usr/bin/env bash

set -ex

: "${BUILD_PROFILE:=}"
package_version="6.32.1"

conan create . -pr:h=${BUILD_PROFILE} -pr:b=${BUILD_PROFILE} --version="${package_version}.ecs" --build=missing
