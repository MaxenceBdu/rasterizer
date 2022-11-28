#include "vector.h"

namespace aline
{
    template <class T, int M, int N>
    class Matrix
    {
        Vector<T, N> vectors[M]; // tableau de pointeurs de vecteurs
    public:
        
        // Constructs a matrix filled up with zeros.
        Matrix()
        {
            for (int i = 0; i < M; ++i)
            {
                vectors[i] = Vector<T,N>();
            }
        }

        // Constructs a matrix with the vectors given as arguments. Each vector is one line of the matrix.
        Matrix(std::initializer_list<Vector<T, N>> l)
        {

            size_t i = 0;
            for (auto &item : l)
            { // fill with values of l
                vectors[i++] = Vector<T,N>(item);
            }
            for (size_t j = i; j < M; ++j)
            { // fill with zeros if l.size() < N
                vectors[j] = Vector<T, N>();
            }
        }

        // Constructs a copy of the matrix given as argument.
        Matrix(const Matrix<T, M, N> &m)
        {
            for (size_t i = 0; i < M; i++)
            {
                vectors[i] = Vector<T,N>(m[i]);
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
    template <class T, int M, int N>
    Matrix<double, M, N> inverse(const Matrix<T, M, N> &m){

        auto copy = Matrix<double,M,N>(); // copy the matrix to permit editing
        for(int i = 0; i < M; ++i)
            for(int j = 0; j < N; ++j)
                copy[i][j] = (double) m[i][j];

        auto identity = Matrix<double,M,N>(); // creation of identity matrix
        for(int i = 0; i < M; ++i)
            for(int j = 0; j < N; ++j)
                if(i == j) identity[i][j] = 1;
            
        for(int i = 0; i < N; ++i){ // Loop on each column

            // search a max != 0 on the column
            double max = -1;
            int row_max = -1;
            for(int j = i; j < M; ++j){
                T x = std::abs(copy[j][i]);
                if(x > 0 && x >= max){
                    max = copy[j][i];
                    row_max = j;
                }                
            }

            if(max != -1){ // max not found, matrix not invertible
                Matrix<double,M,N> nan_matrix = Matrix<double,M,N>();
                for(int k = 0; k < M; ++k){
                    for(int j = 0; j < N; ++j){
                        nan_matrix[k][j] = std::nan("");
                    }
                }
                return nan_matrix;
            }

            // Swap rows
            if(row_max != i){
                Vector<double,N> temp = copy[row_max];
                copy[row_max] = copy[i];
                copy[i] = temp;

                Vector<double,N> temp2 = identity[row_max];
                identity[row_max] = identity[i];
                identity[i] = temp2;
            }

            // Multiply by 1/max
            copy[i] = copy[i] * (1/max);
            identity[i] = identity[i] * (1/max);

            for(int j = 0; j < M; ++j){
                if(j != i){
                    copy[j] = copy[j] + copy[i] * (-copy[j][i]);
                    identity[j] = identity[j] + identity[i] * (-copy[j][i]);
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
        ss << to_string(m[N-1]) << ")";
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