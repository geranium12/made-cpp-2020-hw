#include <cmath>

#include "Point.h"

const extern double kEps;

Point::Point(double x, double y) : x(x), y(y) {}

bool Point::operator==(const Point& point) const {
  return fabs(x - point.x) < kEps &&
      fabs(y - point.y) < kEps;
}

bool Point::operator!=(const Point& point) const {
  return !(point == *this);
}

Point Point::operator-() const {
  return Point(-x, -y);
}

Point Point::operator-(const Point& a) const {
  return Point(x - a.x, y - a.y);
}

void Point::rotate(const Point& center, double angle) {
  angle *= std::acos(-1) / 180;
  double rotatedX = center.x + (x - center.x) * cos(angle) - (y - center.y) * sin(angle);
  double rotatedY = center.y + (y - center.y) * cos(angle) + (x - center.x) * sin(angle);
  x = rotatedX;
  y = rotatedY;
}

void Point::reflex(const Point& center) {
  x = 2 * center.x - x;
  y = 2 * center.y - y;
}

void Point::reflex(const Line& axis) {
  double a = axis.getA();
  double b = axis.getB();
  double k2 = b / a;
  double b2 = y + k2 * x;
  Line normal(k2, b2);
  Point o = axis.cross(normal);
  this->reflex(o);
}

void Point::scale(const Point& center, double coefficient ) {
  x = coefficient * (x - center.x) + center
      .x;
  y = coefficient * (y - center.y) + center
      .y;
}