#!/bin/bash
# Please run the script at the root of source directory.
# Usage:
#   anvil.sh [--build-type=<Debug|Release>] [--no-build]

set -eu

# Use ninja if available
default_generator() {
  command -v ninja >/dev/null 2>&1 \
    && { echo "Ninja"; } \
    || { echo "Unix Makefiles"; }
}

# Global Settings
GENERATOR=${GENERATOR:-"$(default_generator)"}
# CPM currently requires absolute path to work correctly.
DEPS_SOURCE_CACHE=`realpath ${ANVIL_DEPS_CACHE:-~/.cpp-cpm-cache}`

# Commandline Options
BUILD_TYPE="Release"
NO_BUILD="false"

for i in "$@"
do
case $i in
  --build-type=*)
  BUILD_TYPE="${i#*=}"
  shift # past argument=value
  ;;
  --no-build)
  NO_BUILD="true"
  shift # past flag
  ;;
  -h|--help)
  echo "anvil.sh [--build-type=<Debug|Release>] [--no-build]"
  exit
  shift # past flag
  ;;
  *)    # unknown option
  shift # past argument
  ;;
esac
done

echo -e "[*] Configuratons:\n" \
  " GENERATOR=$GENERATOR\n" \
  " DEPS_SOURCE_CACHE=$DEPS_SOURCE_CACHE\n" \
  " --build-type=$BUILD_TYPE\n" \
  " --no-build=$NO_BUILD"

SOURCE_DIR=`pwd`

OUT_PATH=$SOURCE_DIR/build/$BUILD_TYPE

if [[ ! -d $OUT_PATH ]]; then
  mkdir -p $OUT_PATH
fi

cmake \
  -DCPM_SOURCE_CACHE=$DEPS_SOURCE_CACHE \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
  -G "$GENERATOR" \
  -B $OUT_PATH \
  -S $SOURCE_DIR

if [[ $NO_BUILD == "true" ]]; then
  echo "[*] Generate project only, skip build"
  exit 0
fi

cmake \
  --build $OUT_PATH \
  -- -j 8
