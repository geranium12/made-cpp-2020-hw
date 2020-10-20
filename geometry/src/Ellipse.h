#ifndef INC_2_SRC_ELLIPSE_H_
#define INC_2_SRC_ELLIPSE_H_

#include <utility>

#include "Point.h"
#include "Line.h"
#include "Shape.h"

class Point;
class Line;
class Shape;

class Ellipse : public Shape {
 protected:
  Point f1;
  Point f2;
  double semiMajorAxis;

 public:
  explicit Ellipse(const Point& a, const Point& b, double doubleSemiMajorAxis);
  virtual ~Ellipse() = default;

  std::pair<Point, Point> Focuses();
  double Eccentricity();

  virtual double Perimeter() override;
  double Area() override;
  bool operator==(const Shape& another) const override;
  bool operator!=(const Shape& another) const override;

  void Rotate(Point center, double angle) override;
  void Reflex(Point center) override;
  void Reflex(Line axis) override;
  void Scale(Point center, double coefficient) override;
};

#endif //INC_2_SRC_ELLIPSE_H_
