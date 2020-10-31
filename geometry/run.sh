#!/bin/bash

set -e

g++ -std=c++17 -I./src test/test.cpp src/Point.cpp src/Line.cpp src/Shape.cpp src/Polygon.cpp src/Ellipse.cpp src/Circle.cpp src/Rectangle.cpp src/Square.cpp src/Triangle.cpp -o geometry
./geometry

echo All tests passed!