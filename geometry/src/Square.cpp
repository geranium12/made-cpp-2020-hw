#include <vector>
#include <cmath>

#include "Square.h"

Square::Square(const Point& a, const Point& b) : Rectangle(a, b, 1) {}

Circle Square::circumscribedCircle() {
  double diffX = fabs(vertices[0].x - vertices[1].x);
  return Circle(center(), diffX / sqrt(2));
}

Circle Square::inscribedCircle() {
  double diffX = fabs(vertices[0].x - vertices[1].x);
  return Circle(center(), diffX / 2);
}