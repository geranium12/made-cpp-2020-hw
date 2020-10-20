#ifndef INC_2_SRC_SHAPE_H_
#define INC_2_SRC_SHAPE_H_

#include "Point.h"
#include "Line.h"

class Point;
class Line;

class Shape {
 protected:
  double GetDistance(const Point& a, const Point& b);

 public:
  virtual ~Shape() = default;

  virtual double Perimeter() = 0;
  virtual double Area() = 0;
  virtual bool operator==(const Shape& another) const = 0;
  virtual bool operator!=(const Shape& another) const = 0;

  virtual void Rotate(Point center, double angle) = 0;
  virtual void Reflex(Point center) = 0;
  virtual void Reflex(Line axis) = 0;
  virtual void Scale(Point center, double coefficient) = 0;
};

#endif //INC_2_SRC_SHAPE_H_
