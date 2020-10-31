#include <vector>
#include <cmath>

#include "Rectangle.h"

Rectangle::Rectangle(const Point& a, const Point& b, double aspectRatio) : Polygon({a, b}) {
  Point center = getAverage(a, b);
  double angle = atan(aspectRatio) * 180. / atan(-1);
  Point c = vertices[0];
  Point d = vertices[1];
  c.rotate(center, angle / 2);
  d.rotate(center, angle / 2);
  vertices.push_back(c);
  vertices.push_back(d);
  std::swap(vertices[1], vertices[2]);
}

Point Rectangle::center() {
  return getAverage(vertices[0], vertices[1]);
}

std::pair<Line, Line> Rectangle::diagonals() {

  return std::make_pair(Line(vertices[0], vertices[2]),
                        Line(vertices[1], vertices[3]));
}