#!/usr/bin/env bash

clang-format -i `find src -type f -iname "*pp" | xargs`
clang-format -i `find tests -type f -iname "*pp" | xargs`
