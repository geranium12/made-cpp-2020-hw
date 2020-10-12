#pragma once

#include <vector>
#include <iostream>


namespace task {

const double EPS = 1e-6;

class OutOfBoundsException : public std::exception {};
class SizeMismatchException : public std::exception {};

class Matrix {
 private:
  size_t rows;
  size_t cols;
  double** mat;

  class ProxyClass {
   private:
    double* p;
    size_t size;
   public:
    ProxyClass(double* p, size_t size) : p(p), size(size) {}

    double& operator[] (size_t i) {
      if (i >= size) {
        throw OutOfBoundsException();
      }

      return p[i];
    }

    const double& operator[] (size_t i) const {
      if (i >= size) {
        throw OutOfBoundsException();
      }

      return p[i];
    }
  };

  void delMat();
  double detHelper(const size_t& size, double** mat) const;

 public:

  Matrix();
  ~Matrix();
  Matrix(size_t rows, size_t cols);
  Matrix(const Matrix& copy);
  Matrix& operator=(const Matrix& a);

  double& get(size_t row, size_t col);
  const double& get(size_t row, size_t col) const;
  void set(size_t row, size_t col, const double& value);
  void resize(size_t new_rows, size_t new_cols);

  ProxyClass operator[](size_t row);
  ProxyClass operator[](size_t row) const;

  Matrix& operator+=(const Matrix& a);
  Matrix& operator-=(const Matrix& a);
  Matrix& operator*=(const Matrix& a);
  Matrix& operator*=(const double& number);

  Matrix operator+(const Matrix& a) const;
  Matrix operator-(const Matrix& a) const;
  Matrix operator*(const Matrix& a) const;
  Matrix operator*(const double& number) const;

  Matrix operator-() const;
  Matrix operator+() const;

  double det() const;
  void transpose();
  Matrix transposed() const;
  double trace() const;

  std::vector<double> getRow(size_t row) const;
  std::vector<double> getColumn(size_t column) const;

  bool operator==(const Matrix& a) const;
  bool operator!=(const Matrix& a) const;

  size_t getRows() const;
  size_t getCols() const;
};


Matrix operator*(const double& a, const Matrix& b);

std::ostream& operator<<(std::ostream& output, const Matrix& matrix);
std::istream& operator>>(std::istream& input, Matrix& matrix);

}  // namespace task