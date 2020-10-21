#ifndef INC_2_SRC_POINT_H_
#define INC_2_SRC_POINT_H_

#include "Line.h"

class Line;

class Point {
 public:
  double x;
  double y;

  explicit Point(double x, double y);
  ~Point() = default;

  bool operator==(const Point& point) const;
  bool operator!=(const Point& point) const;

  Point operator-() const;
  Point operator-(const Point& a) const;

  void rotate(const Point& center, double angle);
  void reflex(const Point& center);
  void reflex(const Line& axis);
  void scale(const Point& center, double coefficient);
};

#endif //INC_2_SRC_POINT_H_
