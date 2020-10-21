#ifndef INC_2_SRC_SQUARE_H_
#define INC_2_SRC_SQUARE_H_

#include "Point.h"
#include "Rectangle.h"
#include "Circle.h"

class Point;
class Rectangle;
class Circle;

class Square : public Rectangle {
 public:
  explicit Square(const Point& a, const Point& b);
  ~Square() override = default;

  Circle circumscribedCircle();
  Circle inscribedCircle();
};

#endif //INC_2_SRC_SQUARE_H_
