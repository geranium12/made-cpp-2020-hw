#include <cmath>

#include "Ellipse.h"

Ellipse::Ellipse(const Point& f1, const Point& f2, double doubleSemiMajorAxis) : f1(f1), f2(f2), semiMajorAxis(doubleSemiMajorAxis / 2) {}

std::pair<Point, Point> Ellipse::Focuses() {
  return std::make_pair(f1, f2);
}

double Ellipse::eccentricity() {
  double focalLength = getDistance(f1, f2) / 2;
  double eccentricity = focalLength / semiMajorAxis;
  return eccentricity;
}

double Ellipse::perimeter() {
  double integral = std::comp_ellint_2(eccentricity());
  return 4 * semiMajorAxis * integral;
}

double Ellipse::area() {
  double focalLength = getDistance(f1, f2) / 2;
  double semiMinorAxis = semiMajorAxis * semiMajorAxis - focalLength * focalLength;
  semiMinorAxis = sqrt(semiMinorAxis);

  double area = std::acos(-1) * semiMajorAxis * semiMinorAxis;

  return area;
}

bool Ellipse::operator==(const Shape& another) const {
  const Ellipse& anotherEllipse = dynamic_cast<const Ellipse&>(another);

  return f1 == anotherEllipse.f1 &&
      f2 == anotherEllipse.f2 &&
      semiMajorAxis == anotherEllipse.semiMajorAxis;
}

bool Ellipse::operator!=(const Shape& another) const {
  return !(*this == another);
}

void Ellipse::rotate(Point center, double angle) {
  f1.rotate(center, angle);
  f2.rotate(center, angle);
}

void Ellipse::reflex(Point center) {
  f1.reflex(center);
  f2.reflex(center);
}

void Ellipse::reflex(Line axis) {
  f1.reflex(axis);
  f2.reflex(axis);
}

void Ellipse::scale(Point center, double coefficient) {
  f1.scale(center, coefficient);
  f2.scale(center, coefficient);
  semiMajorAxis *= coefficient;
}
