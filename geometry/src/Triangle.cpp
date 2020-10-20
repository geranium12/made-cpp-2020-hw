#include <vector>

#include "Triangle.h"

Triangle::Triangle(const Point& a, const Point& b, const Point& c) : Polygon({a, b, c}) {}

Circle Triangle::CircumscribedCircle() {
  Point a = vertices[0];
  Point b = vertices[1] - a;
  Point c = vertices[2] - a;

  double d = 2 * (b.x * c.y - b.y * c.x);
  double distB = b.x * b.x + b.y * b.y;
  double distC = c.x * c.x + c.y * c.y;
  double centerX = (c.y * distB - b.y * distC) / d;
  double centerY = (b.x * distC - c.x * distB) / d;
  double r = GetDistance(Point(centerX, centerY), Point(0, 0));

  return Circle(Point(centerX + a.x, centerY + a.y), r);
}

Circle Triangle::InscribedCircle() {
  const Point& a = vertices[0];
  const Point& b = vertices[1];
  const Point& c = vertices[2];
  double lengthA = GetDistance(b, c);
  double lengthB = GetDistance(a, c);
  double lengthC = GetDistance(a, b);

  double centerX = lengthA * a.x + lengthB * b.x + lengthC * c.x;
  centerX /= (lengthA + lengthB + lengthC);
  double centerY = lengthA * a.y + lengthB * b.y + lengthC * c.y;
  centerY /= (lengthA + lengthB + lengthC);
  double r = 2 * Area() / Perimeter();

  return Circle(Point(centerX, centerY), r);
}

Point Triangle::Centroid() {
  const Point& a = vertices[0];
  const Point& b = vertices[1];
  const Point& c = vertices[2];

  double centerX = (a.x + b.x + c.x) / 3;
  double centerY = (a.y + b.y + c.y) / 3;

  return Point(centerX, centerY);
}

Point Triangle::Orthocenter() {
  const Point& a = vertices[0];
  const Point& b = vertices[1];
  const Point& c = vertices[2];

  double centerX = a.y * a.y * (c.y - b.y) + b.x * c.x * (c.y - b.y);
  centerX += (b.y * b.y * (a.y - c.y) + a.x * c.x * (a.y - c.y));
  centerX += (c.y * c.y * (b.y - a.y) + a.x * b.x * (b.y - a.y));
  centerX /= (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));

  double centerY = a.x * a.x * (b.x - c.x) + b.y * c.y * (b.x - c.x);
  centerY += (b.x * b.x * (c.x - a.x) + a.y * c.y * (c.x - a.x));
  centerY += (c.x * c.x * (a.x - b.x) + a.y * b.y * (a.x - b.x));
  centerY /= (a.y * (c.x - b.x) + b.y * (a.x - c.x) + c.y * (b.x - a.x));

  return Point(centerX, centerY);
}

Line Triangle::EulerLine() {
  return Line(Centroid(), Orthocenter());
}

Circle Triangle::NinePointsCircle() {
  Point center(GetAverage(Orthocenter(), CircumscribedCircle().Center()));
  Point middleSide = GetAverage(vertices[0], vertices[1]);
  double r = GetDistance(center, middleSide);
  return Circle(center, r);
}