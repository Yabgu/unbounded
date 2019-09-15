#!/bin/bash
set -e

cd $(dirname "$BASH_SOURCE")

if [ ! -d third-party/vcpkg/vcpkg ]; then
  mkdir third-party
  pushd third-party
  git clone https://github.com/Microsoft/vcpkg.git
  popd

  pushd third-party/vcpkg
  ./bootstrap-vcpkg.sh
  popd
fi

third-party/vcpkg/vcpkg install libxml2 gtest

if [ ! -d build ]; then
  mkdir build
fi

pushd build
cmake .. "-DCMAKE_TOOLCHAIN_FILE=../third-party/vcpkg/scripts/buildsystems/vcpkg.cmake"
popd
