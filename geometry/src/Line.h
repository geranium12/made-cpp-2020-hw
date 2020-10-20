#ifndef INC_2_SRC_LINE_H_
#define INC_2_SRC_LINE_H_

#include "Point.h"

class Point;

class Line {
 private:
  double a;
  double b;
  double c;

 public:
  explicit Line(const Point& a, const Point& b);
  explicit Line(double k, double b);
  explicit Line(const Point& a, double k);
  ~Line() = default;

  bool operator==(const Line& line) const;
  bool operator!=(const Line& line) const;

  double GetA() const;
  double GetB() const;

  Point Cross(const Line& line) const;
};

#endif //INC_2_SRC_LINE_H_
