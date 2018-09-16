#!/usr/bin/env bash

clang-format -i `find src -type f | xargs`
clang-format -i `find tests -type f | xargs`
