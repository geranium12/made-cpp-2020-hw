#!/bin/bash

set -e

g++ -std=c++17 -I./ main.cpp -o allocator_test
./allocator_test

echo All tests passed!