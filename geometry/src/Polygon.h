#ifndef INC_2_SRC_POLYGON_H_
#define INC_2_SRC_POLYGON_H_

#include "Point.h"
#include "Line.h"
#include "Shape.h"

class Point;
class Line;
class Shape;

class Polygon : public Shape {
 protected:
  std::vector<Point> vertices;

  double GetTrapezoidArea(const Point& a, const Point& b);

 public:
  Polygon(const std::vector<Point>& vertices);
  virtual ~Polygon() = default;

  size_t verticesCount();
  const std::vector<Point> getVertices() const;

  double Perimeter() override;
  double Area() override;
  bool operator==(const Shape& another) const override;
  bool operator!=(const Shape& another) const override;

  void Rotate(Point center, double angle) override;
  void Reflex(Point center) override;
  void Reflex(Line axis) override;
  void Scale(Point center, double coefficient) override;

  Point GetAverage(const Point& a, const Point& b);
};

#endif //INC_2_SRC_POLYGON_H_
