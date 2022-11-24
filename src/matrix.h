#include "vector.h"

namespace aline
{
    template <class T, int M, int N>
    class Matrix
    {
    public:
        Vector<T, N> *vectors;
        // Constructs a matrix filled up with zeros.
        Matrix()
        {
            vectors = new Vector<T, N>[M];
            for (int i = 0; i < M; ++i)
            {
                vectors[i] = Vector<T,N>();
            }
        }

        // Constructs a matrix with the vectors given as arguments. Each vector is one line of the matrix.
        Matrix(std::initializer_list<Vector<T, N>> l)
        {
            vectors = new Vector<T, N>[M];

            size_t i = 0;
            for (auto &item : l)
            { // fill with values of l
                vectors[i] = Vector<T,N>(item);
                ++i;
            }
            for (size_t j = i; j < N; ++j)
            { // fill with zeros if l.size() < N
                vectors[j] = Vector<T,N>();
            }
        }

        // Constructs a copy of the matrix given as argument.
        Matrix(const Matrix<T, M, N> &m)
        {
            vectors = new Vector<T,N>[M];
            for (size_t i = 0; i < M; i++)
            {
                vectors[i] = m.vectors[i];
            }
        }

        ~Matrix()
        {
            // for(int i = 0; i < M; ++i)[
            //     delete [] vectors[i].vectors;
            //]
            delete[] vectors;
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
    template <class T, int M, int N>
    Matrix<T, M, N> inverse(const Matrix<T, M, N> &m){
        // TO DO
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
            if(!nearly_equal(m1[i], m2[i]))
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
    std::ostream &operator<<(std::ostream& out, const Matrix<T, M, N> &m)
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
            result [i] = m1[i]+m2[i];
            //for (int j = 0; j < N; ++j)
                //result[i][j] = m1[i][j] + m2[i][j];
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
            result [i] = m1[i]-m2[i];
            //for (int j = 0; j < N; ++j)
                //result[i][j] = m1[i][j] - m2[i][j];

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
        for (int i = 0; i < N; ++i)
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
        return (1 / s) * m;
    }

    // A string representation of a matrix.
    template <class T, int M, int N>
    std::string to_string(const Matrix<T, M, N> &m)
    {
        std::stringstream ss;
        ss << "{ ";
        for (int i = 0; i < M; i++)
        {
            ss << to_string(m[i]);
        }
        ss << " }";
        return ss.str();
    }

    // The transpose of a matrix.
    template <class T, int M, int N>
    Matrix<T, M, N> transpose(const Matrix<T, M, N> &m){
        Matrix<T,M,N> result = Matrix<T,M,N>();
        for(int i = 0; i < M; ++i){
            for(int j = 0; j < N; j++){
                result[i][j] = m[j][i];
            }
        }
        return result;
    }
}