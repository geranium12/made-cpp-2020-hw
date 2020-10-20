#include <cmath>

#include "Ellipse.h"

Ellipse::Ellipse(const Point& f1, const Point& f2, double doubleSemiMajorAxis) : f1(f1), f2(f2), semiMajorAxis(doubleSemiMajorAxis / 2) {}

std::pair<Point, Point> Ellipse::Focuses() {
  return std::make_pair(f1, f2);
}

double Ellipse::Eccentricity() {
  double focalLength = GetDistance(f1, f2) / 2;
  double eccentricity = focalLength / semiMajorAxis;
  return eccentricity;
}

double Ellipse::Perimeter() {
  double integral = std::comp_ellint_2(Eccentricity());
  return 4 * semiMajorAxis * integral;
}

double Ellipse::Area() {
  double focalLength = GetDistance(f1, f2) / 2;
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

void Ellipse::Rotate(Point center, double angle) {
  f1.Rotate(center, angle);
  f2.Rotate(center, angle);
}

void Ellipse::Reflex(Point center) {
  f1.Reflex(center);
  f2.Reflex(center);
}

void Ellipse::Reflex(Line axis) {
  f1.Reflex(axis);
  f2.Reflex(axis);
}

void Ellipse::Scale(Point center, double coefficient) {
  f1.Scale(center, coefficient);
  f2.Scale(center, coefficient);
  semiMajorAxis *= coefficient;
}
