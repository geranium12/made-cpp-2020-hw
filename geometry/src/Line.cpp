#include <cmath>

#include "Line.h"

const extern double kEps;

Line::Line(const Point& a, const Point& b) {
  this->a = a.x - b.x;
  this->b = b.x - a.x;
  this->c = a.x * b.y - b.x * a.y;

  if (this->a < 0) {
    this->a *= -1;
    this->b *= -1;
    this->c *= -1;
  }

  if (this->a > kEps) {
    this->b /= this->a;
    this->c /= this->a;
    this->a = 1;
  } else {
    this->a /= this->b;
    this->c /= this->b;
    this->b = 1;
  }
}

Line::Line(double k, double b) : a(-k), b(1), c(-b) {};

Line::Line(const Point& a, double k) : Line(k, a.y - k * a.x) {}

bool Line::operator==(const Line& line) const {
  return fabs(a - line.a) < kEps &&
      fabs(b - line.b) < kEps &&
      fabs (c - line.c) < kEps;
}

bool Line::operator!=(const Line& point) const {
  return !(point == *this);
}

double Line::getA() const {
  return a;
}

double Line::getB() const {
  return b;
}

Point Line::cross(const Line& line) const {
  double x = line.c * b - c * line.b;
  x /= (a * line.b - line.a * b);
  double y = line.a * c - line.c * a;
  y /= (a * line.b - line.a * b);
  return Point(x, y);
}
