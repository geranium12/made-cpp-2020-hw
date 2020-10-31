#ifndef INC_2_SRC_POLYGON_H_
#define INC_2_SRC_POLYGON_H_

#include <vector>

#include "Point.h"
#include "Line.h"
#include "Shape.h"

class Point;
class Line;
class Shape;

class Polygon : public Shape {
 protected:
  std::vector<Point> vertices;

  Point getAverage(const Point& a, const Point& b);
  double getTrapezoidArea(const Point& a, const Point& b);

 public:
  Polygon(const std::vector<Point>& vertices);
  virtual ~Polygon() = default;

  size_t verticesCount();
  const std::vector<Point> getVertices() const;

  double perimeter() override;
  double area() override;
  bool operator==(const Shape& another) const override;
  bool operator!=(const Shape& another) const override;

  void rotate(Point center, double angle) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
  void scale(Point center, double coefficient) override;
};

#endif //INC_2_SRC_POLYGON_H_
