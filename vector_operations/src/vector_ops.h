#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>

extern const double EPS;

namespace task {

struct NotEqualSizeException : public std::exception {
  [[nodiscard]] const char * what () const noexcept override {
    return "Vectors must have equal size!";
  }
};

struct Not3DimSpaceException : public std::exception {
  [[nodiscard]] const char * what () const noexcept override {
    return "Vectors must be in three-dimensional space!";
  }
};

struct NegativeSizeException : public std::exception {
  [[nodiscard]] const char * what () const noexcept override {
    return "Vector size must be non-negative!";
  }
};

std::vector<double> operator+ (const std::vector<double>& v) {
  std::vector<double> ret(v.size());
  for (size_t i = 0; i < ret.size(); ++i) {
    ret[i] = v[i];
  }

  return ret;
}

std::vector<double> operator- (const std::vector<double>& v) {
  std::vector<double> ret(v.size());
  for (size_t i = 0; i < ret.size(); ++i) {
    ret[i] = -v[i];
  }

  return ret;
}

template <class T>
bool isEqualSize(const std::vector<T>& v1, const std::vector<T>& v2) {
  if (v1.size() == v2.size()) {
    return true;
  }

  return false;
}

std::vector<double> operator+ (const std::vector<double>& v1, const std::vector<double>& v2) {
  if (!isEqualSize(v1, v2)) {
    throw NotEqualSizeException();
  }

  std::vector<double> ret(v1.size());
  for (size_t i = 0; i < v1.size(); ++i) {
    ret[i] = v1[i];
  }

  for (size_t i = 0; i < v2.size(); ++i) {
    ret[i] += v2[i];
  }

  return ret;
}

std::vector<double> operator- (const std::vector<double>& v1, const std::vector<double>& v2) {
  if (!isEqualSize(v1, v2)) {
    throw NotEqualSizeException();
  }

  return -v2 + v1;
}

double operator* (const std::vector<double>& v1, const std::vector<double>& v2) {
  if (!isEqualSize(v1, v2)) {
    throw NotEqualSizeException();
  }

  double ret = 0;
  size_t minSize = std::min(v1.size(), v2.size());
  for (size_t i = 0; i < minSize; ++i) {
    ret += v1[i] * v2[i];
  }

  return ret;
}

std::vector<double> operator% (const std::vector<double>& v1, const std::vector<double>& v2) {
  if (!isEqualSize(v1, v2)) {
    throw NotEqualSizeException();
  }

  bool is3DimSpace = (v1.size() == 3);
  if (!is3DimSpace) {
    throw Not3DimSpaceException();
  }

  std::vector<double> ret;
  ret.push_back(v1[1] * v2[2] - v1[2] * v2[1]);
  ret.push_back(v1[2] * v2[0] - v1[0] * v2[2]);
  ret.push_back(v1[0] * v2[1] - v1[1] * v2[0]);

  return ret;
}

double getLength(const std::vector<double>& v) {
  double length = 0;
  for (double coord : v) {
    length += coord * coord;
  }

  return length;
}

bool operator|| (const std::vector<double>& v1, const std::vector<double>& v2) {
  if (!isEqualSize(v1, v2)) {
    throw NotEqualSizeException();
  }

  double length1 = getLength(v1);
  double length2 = getLength(v2);
  double dotProduct = v1 * v2;
  if (fabs(fabs(dotProduct) - sqrt(length1 * length2)) < EPS) {
    return true;
  }

  return false;
}

bool operator&& (const std::vector<double>& v1, const std::vector<double>& v2) {
  if (!isEqualSize(v1, v2)) {
    throw NotEqualSizeException();
  }

  double length1 = getLength(v1);
  double length2 = getLength(v2);
  double dotProduct = v1 * v2;
  if (-dotProduct + sqrt(length1 * length2) < EPS) {
    return true;
  }

  return false;
}

std::istream& operator>> (std::istream& is, std::vector<double>& v) {
  int size;
  is >> size;
  if (size < 0) {
    throw NegativeSizeException();
  }
  v.clear();
  if (size > 0) {
    v.resize(size);
    for (size_t i = 0; i < size; ++i) {
      is >> v[i];
    }
  }
  return is;
}

std::ostream& operator<< (std::ostream& os, const std::vector<double>& v) {
  for (double elem : v) {
    os << elem << ' ';
  }
  os << std::endl;

  return os;
}

void reverse(std::vector<double>& v) {
  std::vector<double> ret(v.size());
  for (size_t i = 1; i <= v.size(); ++i) {
    ret[i - 1] = v[v.size() - i];
  }

  v.clear();
  v = ret;
}

std::vector<int> operator| (const std::vector<int>& v1, const std::vector<int>& v2) {
  if (!isEqualSize(v1, v2)) {
    throw NotEqualSizeException();
  }

  std::vector<int> ret(v1.size());
  for (size_t i = 0; i < v1.size(); ++i) {
    ret[i] = v1[i] | v2[i];
  }

  return ret;
}

std::vector<int> operator& (const std::vector<int>& v1, const std::vector<int>& v2) {
  if (!isEqualSize(v1, v2)) {
    throw NotEqualSizeException();
  }

  std::vector<int> ret(v1.size());
  for (size_t i = 0; i < v1.size(); ++i) {
    ret[i] = v1[i] & v2[i];
  }
  return ret;
}
}  // namespace task