#ifndef INC_2_SRC_CIRCLE_H_
#define INC_2_SRC_CIRCLE_H_

#include "Point.h"
#include "Ellipse.h"

class Point;
class Ellipse;

class Circle : public Ellipse {
 public:
  explicit Circle(const Point& center, double radius);
  ~Circle() override = default;

  Point center();
  double radius();
};

#endif //INC_2_SRC_CIRCLE_H_
