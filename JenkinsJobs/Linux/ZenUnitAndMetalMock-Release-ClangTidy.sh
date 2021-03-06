#!/bin/bash
set -ev

export CXX=/usr/bin/clang++
cmake -H. -BRelease -GNinja -DCMAKE_BUILD_TYPE=Release -DClangTidyMode=ON
(cd Release; time ninja clang-tidy)
