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

void Point::Rotate(const Point& center, double angle) {
  angle *= std::acos(-1) / 180;
  double rotatedX = center.x + (x - center.x) * cos(angle) - (y - center.y) * sin(angle);
  double rotatedY = center.y + (y - center.y) * cos(angle) + (x - center.x) * sin(angle);
  x = rotatedX;
  y = rotatedY;
}

void Point::Reflex(const Point& center) {
  x = 2 * center.x - x;
  y = 2 * center.y - y;
}

void Point::Reflex(const Line& axis) {
  double a = axis.GetA();
  double b = axis.GetB();
  double k2 = b / a;
  double b2 = y + k2 * x;
  Line normal(k2, b2);
  Point o = axis.Cross(normal);
  this->Reflex(o);
}

void Point::Scale(const Point& center, double coefficient ) {
  x = coefficient * (x - center.x) + center
      .x;
  y = coefficient * (y - center.y) + center
      .y;
}