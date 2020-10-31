#include <cmath>

#include "Shape.h"

double Shape::getDistance(const Point& a, const Point& b) {
  double dist = (b.x - a.x) * (b.x - a.x);
  dist += (b.y - a.y) * (b.y - a.y);
  return sqrt(dist);
}
