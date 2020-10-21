#ifndef INC_2_SRC_SHAPE_H_
#define INC_2_SRC_SHAPE_H_

#include "Point.h"
#include "Line.h"

class Point;
class Line;

class Shape {
 protected:
  double getDistance(const Point& a, const Point& b);

 public:
  virtual ~Shape() = default;

  virtual double perimeter() = 0;
  virtual double area() = 0;
  virtual bool operator==(const Shape& another) const = 0;
  virtual bool operator!=(const Shape& another) const = 0;

  virtual void rotate(Point center, double angle) = 0;
  virtual void reflex(Point center) = 0;
  virtual void reflex(Line axis) = 0;
  virtual void scale(Point center, double coefficient) = 0;
};

#endif //INC_2_SRC_SHAPE_H_
