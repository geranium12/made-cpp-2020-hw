#ifndef INC_2_SRC_TRIANGLE_H_
#define INC_2_SRC_TRIANGLE_H_

#include "Point.h"
#include "Line.h"
#include "Circle.h"
#include "Polygon.h"

class Point;
class Line;
class Circle;
class Polygon;

class Triangle : public Polygon {
 public:
  explicit Triangle(const Point& a, const Point& b, const Point& c);
  ~Triangle() = default;

  Circle circumscribedCircle();
  Circle inscribedCircle();

  Point centroid();
  Point orthocenter();
  Line EulerLine();
  Circle ninePointsCircle();
};

#endif //INC_2_SRC_TRIANGLE_H_
