#include <vector>
#include <cmath>

#include "Polygon.h"

double Polygon::GetTrapezoidArea(const Point& a, const Point& b) {
  double area = a.x + b.x;
  area *= (a.y - b.y);
  area /= 2;
  return area;
}

Polygon::Polygon(const std::vector<Point>& vertices) : vertices(vertices) {}

const std::vector<Point> Polygon::getVertices() const {
  return vertices;
}

size_t Polygon::verticesCount() {
  return vertices.size();
}

double Polygon::Perimeter() {
  double dist = 0;
  for (size_t i = 1; i < vertices.size(); ++i) {
    dist += GetDistance(vertices[i - 1], vertices[i]);
  }
  dist += GetDistance(vertices[0], vertices[vertices.size() - 1]);

  return dist;
}

double Polygon::Area() {
  double area = 0;
  for (size_t i = 1; i < vertices.size(); ++i) {
    area += GetTrapezoidArea(vertices[i - 1], vertices[i]);
  }
  area += GetTrapezoidArea(vertices[vertices.size() - 1], vertices[0]);

  return fabs(area);
}

bool Polygon::operator==(const Shape& another) const {
  const Polygon& anotherPolygon = dynamic_cast<const Polygon&>(another);

  if (vertices.size() != anotherPolygon.vertices.size()) {
    return false;
  }

  int i = 0;
  while (i < vertices.size() && vertices[i] != anotherPolygon.vertices[0]) {
    ++i;
  }
  if (i == vertices.size()) {
    return false;
  }

  if (i + 1 != vertices.size() && vertices[i + 1] == anotherPolygon.vertices[1]) {
    for (int j = 0; j < vertices.size(); ++j) {
      int k = i + j < vertices.size() ? (i + j) : (i + j - vertices.size());
      if (vertices[k] != anotherPolygon.vertices[j]) {
        return false;
      }
    }
  } else {
    for (int j = 0; j < vertices.size(); ++j) {
      int k = i - j >= 0 ? (i - j) : (vertices.size() + i - j);
      if (vertices[k] != anotherPolygon.vertices[j]) {
        return false;
      }
    }
  }

  return true;
}

bool Polygon::operator!=(const Shape& another) const {
  return !(another == *this);
}

void Polygon::Rotate(Point center, double angle) {
  for (auto& vertex : vertices) {
    vertex.Rotate(center, angle);
  }
}

void Polygon::Reflex(Point center) {
  for (auto& vertex : vertices) {
    vertex.Reflex(center);
  }
}

void Polygon::Reflex(Line axis) {
  for (auto& vertex : vertices) {
    vertex.Reflex(axis);
  }
}

void Polygon::Scale(Point center, double coefficient) {
  for (auto& vertex : vertices) {
    vertex.Scale(center, coefficient);
  }
}

Point Polygon::GetAverage(const Point& a, const Point& b) {
  double middleX = (a.x + b.x) / 2;
  double middleY = (a.y + b.y) / 2;
  return Point(middleX, middleY);
}

