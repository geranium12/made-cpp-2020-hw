#include "Circle.h"

Circle::Circle(const Point& center, double r) : Ellipse(center, center, 2 * r) {}

Point Circle::Center() {
  return f1;
}

double Circle::Radius() {
  return semiMajorAxis;
}
