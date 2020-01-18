#! /bin/sh

set -ex

mkdir -p .build
cd .build
cmake ..
cmake --build .