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

  Point center();
  std::pair<Line, Line> diagonals();
};

#endif //INC_2_SRC_RECTANGLE_H_
