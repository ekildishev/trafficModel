#! /bin/sh

set -ex

mkdir -p .build
cd .build
cmake .. -DBUILD_TESTS=On
cmake --build .
./tests