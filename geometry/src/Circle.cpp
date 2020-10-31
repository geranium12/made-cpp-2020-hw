#include "Circle.h"

Circle::Circle(const Point& center, double r) : Ellipse(center, center, 2 * r) {}

Point Circle::center() {
  return f1;
}

double Circle::radius() {
  return semiMajorAxis;
}
