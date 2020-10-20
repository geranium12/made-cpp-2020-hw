#ifndef INC_2_SRC_RECTANGLE_H_
#define INC_2_SRC_RECTANGLE_H_

#include <utility>

#include "Point.h"
#include "Line.h"
#include "Polygon.h"

class Point;
class Line;
class Polygon;

class Rectangle : public Polygon {
 public:
  explicit Rectangle(const Point& a, const Point& b, double aspectRatio);
  virtual ~Rectangle() = default;

  Point Center();
  std::pair<Line, Line> Diagonals();
};

#endif //INC_2_SRC_RECTANGLE_H_
