#include <vector>
#include <cmath>

#include "Square.h"

Square::Square(const Point& a, const Point& b) : Rectangle(a, b, 1) {}

Circle Square::CircumscribedCircle() {
  double diffX = fabs(vertices[0].x - vertices[1].x);
  return Circle(Center(), diffX / sqrt(2));
}

Circle Square::InscribedCircle() {
  double diffX = fabs(vertices[0].x - vertices[1].x);
  return Circle(Center(), diffX / 2);
}