#include "matrix.h"
#include <cmath>

using task::Matrix;

void Matrix::delMat() {
  if (rows > 0) {
    if (cols > 0) {
      for (size_t i = 0; i < rows; ++i) {
        delete[] mat[i];
      }
    }
    delete[] mat;
  }
}

double Matrix::detHelper(const size_t& size, double** mat) const {
  if (size == 1) {
    return mat[0][0];
  }

  double ret = 0;
  for (size_t i = 0; i < size; ++i) {
    double** temp = new double* [size - 1];
    for (size_t j = 0; j < size - 1; ++j) {
      temp[j] = new double[size - 1];
      for (size_t k = 0; k < size - 1; ++k) {
        if (j < i) {
          temp[j][k] = mat[j][k + 1];
        } else {
          temp[j][k] = mat[j + 1][k + 1];
        }
      }
    }

    double add = mat[i][0] * detHelper(size - 1, temp);
    if (i % 2) {
      ret -= add;
    } else {
      ret += add;
    }

    for (size_t j = 0; j < size - 1; ++j) {
      delete[] temp[j];
    }
    delete[] temp;
  }

  return ret;
}

Matrix::Matrix() {
  rows = cols = 1;
  mat = new double*[1];
  mat[0] = new double[1];
  mat[0][0] = 1;
}

Matrix::~Matrix() {
  delMat();
}

Matrix::Matrix(size_t rows, size_t cols) {
  this->rows = rows;
  this->cols = cols;

  mat = new double*[rows];
  for (size_t i = 0; i < rows; ++i) {
    mat[i] = new double[cols];
  }

  for (size_t i = 0; i < rows && i < cols; ++i) {
    mat[i][i] = 1;
  }
}

Matrix::Matrix(const Matrix& copy) {
  rows = copy.rows;
  cols = copy.cols;
  mat = new double*[rows];

  for (size_t i = 0; i < rows; ++i) {
    mat[i] = new double[cols];
  }

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      mat[i][j] = copy.mat[i][j];
    }
  }
}

Matrix& Matrix::operator=(const Matrix& a) {
  if (&a == this) {
    return *this;
  }

  delMat();
  rows = a.rows;
  cols = a.cols;

  mat = new double* [rows];
  for (size_t i = 0; i < rows; ++i) {
    mat[i] = new double[cols];
    for (size_t j = 0; j < cols; ++j)
      mat[i][j] = a.mat[i][j];
  }

  return *this;
}

double& Matrix::get(size_t row, size_t col) {
  if (row >= rows || col >= cols) {
    throw OutOfBoundsException();
  }

  return mat[row][col];
}

const double& Matrix::get(size_t row, size_t col) const {
  if (row >= rows || col >= cols) {
    throw OutOfBoundsException();
  }

  return mat[row][col];
}

void Matrix::set(size_t row, size_t col, const double& value) {
  if (row >= rows || col >= cols) {
    throw OutOfBoundsException();
  }

  mat[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
  double** temp = new double* [new_rows];
  for (size_t i = 0; i < new_rows; ++i) {
    temp[i] = new double[new_cols];
    for (size_t j = 0; j < new_cols; ++j) {
      if (i < rows && j < cols)
        temp[i][j] = mat[i][j];
      else
        temp[i][j] = 0;
    }
  }

  delMat();
  rows = new_rows;
  cols = new_cols;
  mat = temp;
}

Matrix::ProxyClass Matrix::operator[](size_t row) {
  if (row >= rows) {
    throw OutOfBoundsException();
  }

  ProxyClass proxy = ProxyClass(mat[row], cols);
  return proxy;
}

Matrix::ProxyClass Matrix::operator[](size_t row) const {
  if (row >= rows) {
    throw OutOfBoundsException();
  }

  ProxyClass proxy = ProxyClass(mat[row], cols);
  return proxy;
}

Matrix& Matrix::operator+=(const Matrix& a) {
  if (rows != a.rows || cols != a.cols) {
    throw SizeMismatchException();
  }

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      mat[i][j] += a.mat[i][j];
    }
  }

  return *this;
}

Matrix& Matrix::operator-=(const Matrix& a) {
  if (rows != a.rows || cols != a.cols) {
    throw SizeMismatchException();
  }

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      mat[i][j] -= a.mat[i][j];
    }
  }

  return *this;
}

Matrix& Matrix::operator*=(const Matrix& a) {
  if (cols != a.rows) {
    throw SizeMismatchException();
  }

  Matrix* matrix = new Matrix(rows, a.cols);
  double curValue;
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < a.cols; ++j) {
      curValue = 0;
      for (size_t k = 0; k < cols; ++k) {
        curValue += mat[i][k] * a.mat[k][j];
      }
      matrix->mat[i][j] = curValue;
    }
  }
  *this = *matrix;

  return *this;
}

Matrix& Matrix::operator*=(const double& number) {
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; j++) {
      mat[i][j] *= number;
    }
  }

  return *this;
}

Matrix Matrix::operator+(const Matrix& a) const {
  if (rows != a.rows || cols != a.cols) {
    throw SizeMismatchException();
  }

  Matrix* matrix = new Matrix(rows, cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      matrix->mat[i][j] = mat[i][j] + a.mat[i][j];
    }
  }

  return *matrix;
}

Matrix Matrix::operator-(const Matrix& a) const {
  if (rows != a.rows || cols != a.cols) {
    throw SizeMismatchException();
  }

  Matrix* matrix = new Matrix(rows, cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      matrix->mat[i][j] = mat[i][j] - a.mat[i][j];
    }
  }

  return *matrix;
}

task::Matrix Matrix::operator*(const Matrix& a) const {
  if (cols != a.rows) {
    throw SizeMismatchException();
  }

  Matrix* matrix = new Matrix(rows, a.cols);
  double curValue;
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < a.cols; ++j) {
      curValue = 0;
      for (size_t k = 0; k < cols; ++k) {
        curValue += mat[i][k] * a.mat[k][j];
      }
      matrix->mat[i][j] = curValue;
    }
  }

  return *matrix;
}

Matrix Matrix::operator*(const double& number) const {
  Matrix* matrix = new Matrix(rows, cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; j++) {
      matrix->mat[i][j] = mat[i][j] * number;
    }
  }

  return *matrix;
}

Matrix Matrix::operator-() const {
  Matrix* matrix = new Matrix(rows, cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; j++) {
      matrix->mat[i][j] = -mat[i][j];
    }
  }

  return *matrix;
}

Matrix Matrix::operator+() const {
  Matrix* matrix = new Matrix(rows, cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; j++) {
      matrix->mat[i][j] = mat[i][j];
    }
  }

  return *matrix;
}

double Matrix::det() const {
  if (rows != cols || !rows) {
    throw SizeMismatchException();
  }

  return detHelper(rows, mat);
}

void Matrix::transpose() {
  *this = this->transposed();
}

Matrix Matrix::transposed() const {
  Matrix* matrix = new Matrix(cols, rows);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      matrix->mat[j][i] = mat[i][j];
    }
  }

  return *matrix;
}

double Matrix::trace() const {
  if (rows != cols || !rows) {
    throw SizeMismatchException();
  }

  double result = 0;
  for (size_t i = 0; i < rows; ++i) {
    result += mat[i][i];
  }

  return result;
}

std::vector<double> Matrix::getRow(size_t row) const {
  if (row >= rows) {
    throw OutOfBoundsException();
  }

  std::vector<double> v(cols);
  for (size_t j = 0; j < cols; ++j) {
    v[j] = mat[row][j];
  }

  return v;
}

std::vector<double> Matrix::getColumn(size_t column) const{
  if (column >= cols) {
    throw OutOfBoundsException();
  }

  std::vector<double> v(rows);
  for (size_t i = 0; i < rows; ++i) {
    v[i] = mat[i][column];
  }

  return v;
}

bool Matrix::operator==(const Matrix& a) const {
  if (rows != a.rows || cols != a.cols) {
    return false;
  }

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (fabs(mat[i][j] - a.mat[i][j]) >= EPS) {
        return false;
      }
    }
  }

  return true;
}

bool Matrix::operator!=(const Matrix& a) const {
  return !(*this == a);
}

size_t Matrix::getRows() const {
  return rows;
}

size_t Matrix::getCols() const {
  return cols;
}


Matrix task::operator*(const double& a, const Matrix& b) {
  return b * a;
}

std::ostream& task::operator<<(std::ostream& output, const Matrix& matrix) {
  size_t rows = matrix.getRows();
  size_t cols = matrix.getCols();

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      output << matrix[i][j] << " ";
    }
    output << '\n';
  }

  return output;
}

std::istream& task::operator>>(std::istream& input, Matrix& matrix) {
  size_t rows, cols;
  input >> rows >> cols;

  matrix.resize(rows, cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      input >> matrix[i][j];
    }
  }

  return input;
}
