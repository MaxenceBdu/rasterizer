#include "vector.h"

namespace aline
{
  template <class T, int M, int N>
  class Matrix
  {
    Vector<T, N> vectors[M];

  public:
    // Constructs a matrix filled up with zeros.
    Matrix()
    {
      for (int i = 0; i < M; ++i)
      {
        vectors[i] = Vector<T, N>();
      }
    }

    // Constructs a matrix with the vectors given as arguments. Each vector is one line of the matrix.
    Matrix(std::initializer_list<Vector<T, N>> l)
    {

      size_t i = 0;
      for (auto &item : l)
      { // fill with values of l
        vectors[i++] = Vector<T, N>(item);
      }
      for (size_t j = i; j < M; ++j)
      { // fill with zeros if l.size() < N
        vectors[j] = Vector<T, N>();
      }
    }

    // Constructs a copy of the matrix given as argument.
    Matrix(const Matrix<T, M, N> &m)
    {
      for (int i = 0; i < M; i++)
      {
        vectors[i] = Vector<T, N>(m[i]);
      }
    }

    // The line indexed by the given argument. Throws runtime_error if the index is out of range
    Vector<T, N> at(size_t i)
    {
      if (i >= M)
      {
        throw std::runtime_error("Index " + std::to_string(i) + " is out of range");
      }

      return vectors[i];
    }

    // The element indexed by the given arguments. Throws runtime_error if the index is out of range.        *
    T at(size_t i, size_t j) const
    {
      if (i >= M || j >= N)
      {
        throw std::runtime_error("Indexes are out of range");
      }
      return vectors[i][j];
    }

    // Subscripting (the as at(), but does not throw an exception).
    Vector<T, N> operator[](size_t i) const
    {
      return vectors[i];
    }

    // Subscripting permitting assignment.
    Vector<T, N> &operator[](size_t i)
    {
      return vectors[i];
    }

    // Matrix addition and assignment.
    Matrix<T, M, N> &operator+=(const Matrix<T, M, N> &m)
    {
      for (int i = 0; i < M; ++i)
      {
        for (int j = 0; j < N; ++j)
        {
          vectors[i][j] = vectors[i][j] + m[i][j];
        }
      }
      return *this;
    }
  };

  // The inverse of a matrix. If the matrix is not invertible, returns a NAN (not a number) matrix.
  // Using Gauss-Jordan Elimination
  template <class T, int M>
  Matrix<double, M, M> inverse(const Matrix<T, M, M> &m)
  {

    // Copy of m, to edit
    auto copy = Matrix<double, M, M>();
    for (int i = 0; i < M; ++i)
      for (int j = 0; j < M; ++j)
        copy[i][j] = (double)m[i][j];

    // Identity matrix
    auto identity = Matrix<double, M, M>();
    for (int i = 0; i < M; ++i)
      for (int j = 0; j < M; ++j)
        if (i == j)
          identity[i][j] = 1;

    // For each column of m
    for (int j = 0; j < M; ++j)
    {

      // Find the row i ≥ j containing the greatest nonzero absolute value of M′[i][j]
      int row_max = j;
      double max = copy[j][j];
      for (int i = j; i < M; ++i)
      {
        if (std::abs(copy[i][j]) > max)
        {
          max = copy[i][j];
          row_max = i;
        }
      }

      // If max == 0, matrix is not invertible
      if (max == 0)
      {
        return nan_matrix(copy);
      }

      // Swap row row_max and j
      if (row_max != j)
      {
        std::swap(copy[j], copy[row_max]);
        std::swap(identity[j], identity[row_max]);
      }

      // Multiply row j by 1/max
      copy[j] = copy[j] * (1 / max);
      identity[j] = identity[j] * (1 / max);

      // for each row r != row j, row r = row r + row j * -copy[r][j]
      for (int r = 0; r < M; ++r)
      {
        if (r != j)
        {
          double val = -(copy[r][j]);
          copy[r] = copy[r] + copy[j] * val;
          identity[r] = identity[r] + identity[j] * val;
        }
      }
    }

    return identity;
  }

  // Tests if a matrix contains NAN (not a number) values.
  template <class T, int M, int N>
  bool isnan(const Matrix<T, M, N> &m)
  {
    for (int i = 0; i < M; ++i)
      if (isnan(m[i]))
        return true;
    return false;
  }

  // Tests if two vectors contain nearly equal values. Two values are nearly equal when
  // they are very close, with respect to their magnitudes. For example, 1.0000001 can be
  // considered nearly equal to 1, whereas 1.234 is not nearly equal to 1.242. However,
  // because of their magnitude, 67329.234 can be considered nearly equal to 67329.242.1
  template <class T, int N, int M>
  bool nearly_equal(const Matrix<T, M, N> &m1, const Matrix<T, M, N> &m2)
  {

    for (size_t i = 0; i < M; ++i)
      if (!nearly_equal(m1[i], m2[i]))
        return false;

    return true;
  }

  // Tests if two matrices contain the same values.
  template <class T, int M, int N>
  bool operator==(const Matrix<T, M, N> &m1, const Matrix<T, M, N> &m2)
  {
    for (int i = 0; i < M; i++)
      if (m1[i] != m2[i])
        return false;
    return true;
  }

  // Tests if two matrices contain different elements.
  template <class T, int M, int N>
  bool operator!=(const Matrix<T, M, N> &m1, const Matrix<T, M, N> &m2)
  {
    for (int i = 0; i < M; i++)
      if (m1[i] == m2[i])
        return false;
    return true;
  }

  // Output operator.
  template <class T, int M, int N>
  std::ostream &operator<<(std::ostream &out, const Matrix<T, M, N> &m)
  {
    out << to_string(m) << std::endl;
    return out;
  }

  // The sum of two matrices.
  template <class T, int M, int N>
  Matrix<T, M, N> operator+(const Matrix<T, M, N> &m1, const Matrix<T, M, N> &m2)
  {
    Matrix<T, M, N> result = Matrix<T, M, N>();
    for (int i = 0; i < M; ++i)
      result[i] = m1[i] + m2[i];
    // for (int j = 0; j < M;; ++j)
    // result[i][j] = m1[i][j] + m2[i][j];
    return result;
  }

  // The negation of a matrix.
  template <class T, int M, int N>
  Matrix<T, M, N> operator-(const Matrix<T, M, N> &m)
  {
    return (-1) * m;
  }

  // The subtraction of two matrices.
  template <class T, int M, int N>
  Matrix<T, M, N> operator-(const Matrix<T, M, N> &m1, const Matrix<T, M, N> &m2)
  {
    Matrix<T, M, N> result = Matrix<T, M, N>();
    for (int i = 0; i < M; ++i)
      result[i] = m1[i] - m2[i];
    // for (int j = 0; j < N; ++j)
    // result[i][j] = m1[i][j] - m2[i][j];

    return result;
  }

  // The product of a scalar and a matrix.
  template <class T, int M, int N>
  Matrix<T, M, N> operator*(const T &s, const Matrix<T, M, N> &m)
  {
    Matrix<T, M, N> result = Matrix<T, M, N>();
    for (int i = 0; i < M; ++i)
      result[i] = s * m[i];

    return result;
  }

  // The product of a matrix and a scalar.
  template <class T, int M, int N>
  Matrix<T, M, N> operator*(const Matrix<T, M, N> &m, const T &s)
  {
    return s * m;
  }

  // The product of a matrix and a vector.
  template <class T, int M, int N>
  Vector<T, M> operator*(const Matrix<T, M, N> &m, const Vector<T, N> &v)
  {
    Vector<T, M> result = Vector<T, M>();

    for (int i = 0; i < M; ++i)
    {
      Vector<T, N> prod = v * m[i];
      for (int j = 0; j < N; ++j)
      {
        result[i] += prod[j];
      }
    }
    return result;
  }

  // The product of two matrices.
  template <class T, int M, int N, int O>
  Matrix<T, M, O> operator*(const Matrix<T, M, N> &m1, const Matrix<T, N, O> &m2)
  {
    Matrix<T, M, O> result = Matrix<T, M, O>();
    for (int i = 0; i < M; ++i)
    {
      Vector<T, N> v1 = m1[i];
      for (int j = 0; j < O; ++j)
      {
        Vector<T, N> v2 = Vector<T, N>();
        for (int k = 0; k < N; ++k)
        {
          v2[k] = m2[k][j];
        }
        Vector<T, N> prod = v1 * v2;
        for (int p = 0; p < N; ++p)
        {
          result[i][j] += prod[p];
        }
      }
    }

    return result;
  }

  // The division of a matrix by a scalar (same as the multiplication by 1/s ).
  template <class T, int M, int N>
  Matrix<T, M, N> operator/(const Matrix<T, M, N> &m, const T &s)
  {
    if (s == 0)
      return nan_matrix(m);
    return (1 / s) * m;
  }

  // A string representation of a matrix.
  template <class T, int M, int N>
  std::string to_string(const Matrix<T, M, N> &m)
  {
    std::stringstream ss;
    ss << "(";
    for (int i = 0; i < M - 1; i++)
    {
      ss << to_string(m[i]) << ", ";
    }
    ss << to_string(m[N - 1]) << ")";
    return ss.str();
  }

  // The transpose of a matrix.
  template <class T, int M, int N>
  Matrix<T, M, N> transpose(const Matrix<T, M, N> &m)
  {
    Matrix<T, M, N> result = Matrix<T, M, N>();
    for (int i = 0; i < M; ++i)
    {
      for (int j = 0; j < N; j++)
      {
        result[i][j] = m[j][i];
      }
    }
    return result;
  }

  // The transpose of a matrix.
  template <class T, int M, int N>
  Matrix<T, M, N> nan_matrix(const Matrix<T, M, N> &m)
  {
    auto result = Matrix<T, M, N>();
    for (int i = 0; i < M; ++i)
    {
      for (int j = 0; j < N; j++)
      {
        result[i][j] = (T)std::nan("");
      }
    }
    return result;
  }
}