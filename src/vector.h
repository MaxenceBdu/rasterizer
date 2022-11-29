#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <float.h>

namespace aline
{

    template <class T, int N>
    class Vector
    {
        T elmts[N];
    public:

        // default constructor
        Vector()
        {
            for (int i = 0; i < N; i++)
                elmts[i] = 0;
        }

        // Initialize with list of values
        Vector(std::initializer_list<T> l)
        {
            if (l.size() > (size_t)N)
                throw std::runtime_error("Initializer list is too long");


            size_t i = 0;
            for (auto &item : l)
            { // fill with values of l
                elmts[i] = item;
                ++i;
            }
            for (size_t j = i; j < N; ++j)
            { // fill with zeros if l.size() < N
                elmts[j] = 0;
            }
        }

        // Copy constructor
        Vector(const Vector<T, N> &v)
        {
            for (int i = 0; i < N; i++)
            {
                elmts[i] = v.elmts[i];
            }
        }

        T at(size_t i) const
        {
            if (i >= N)
                throw std::runtime_error("Index " + std::to_string(i) + " is out of range");

            return elmts[i];
        }

        T operator[](size_t i) const
        {
            return elmts[i];
        }

        T &operator[](size_t i)
        {
            return elmts[i];
        }

        Vector<T, N> &operator+=(const Vector<T, N> &v)
        {
            for (size_t i = 0; i < N; i++)
            {
                elmts[i] = elmts[i] + v[i];
            }

            return *this;
        }
    };

    // The cross product of two vectors. Uses only the first 3 elements (zero the others in
    // the result). Throws runtime_error if the vectors have less than 3 elements.
    template <class T, int N>
    Vector<T, N> cross(const Vector<T, N> &u, const Vector<T, N> &v)
    {
        if (N < 3)
            throw std::runtime_error("Vectors size is inferior to 3");

        Vector<T, N> vec = Vector<T, N>({u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]});
        for (size_t i = 3; i < N; ++i)
        {
            vec[i] = 0;
        }
        return vec;
    }

    // The dot product of two vectors.
    template <class T, int N>
    T dot(const Vector<T, N> &u, const Vector<T, N> &v)
    {
        T result = 0;
        for (size_t i = 0; i < N; i++)
        {
            result += u[i] * v[i];
        }
        return result;
    }

    // Tests if the vector contains NAN (not a number) values. (It is sometimes useful when
    // the result of a computation does not exist, e.g. division by zero).
    template <class T, int N>
    bool isnan(const Vector<T, N> &v)
    {
        for (size_t i = 0; i < N; i++)
        {
            if (std::isnan(v[i]))
                return true;
        }
        return false;
    }

    // Tests if the vector is a unit vector.
    template <class T, int N>
    bool is_unit(const Vector<T, N> &v)
    {
        return std::round(norm(v)) == 1;
    }

    // Tests if two vectors contain nearly equal values. Two values are nearly equal when
    // they are very close, with respect to their magnitudes. For example, 1.0000001 can be
    // considered nearly equal to 1, whereas 1.234 is not nearly equal to 1.242. However,
    // because of their magnitude, 67329.234 can be considered nearly equal to 67329.242.1
    template <class T, int N>
    bool nearly_equal(const Vector<T, N> &u, const Vector<T, N> &v)
    {

        for (size_t i = 0; i < N; ++i)
        {
            float diff = fabs(u[i] - v[i]);
            float A = fabs(u[i]);
            float B = fabs(v[i]);

            // Find the largest
            float largest = (B > A) ? B : A;

            if (diff > largest * FLT_EPSILON)
                return false;
        }
        return true;
    }

    // The norm (magnitude) of the vector.
    template <class T, int N>
    double norm(const Vector<T, N> &v)
    {
        return sqrt(dot(v, v));
    }

    // Tests if two vectors contain the same values.
    template <class T, int N>
    bool operator==(const Vector<T, N> &u, const Vector<T, N> &v)
    {
        for (size_t i = 0; i < N; i++)
            if (u[i] != v[i])
                return false;

        return true;
    }

    // Test if two vectors contain different values.
    template <class T, int N>
    bool operator!=(const Vector<T, N> &u, const Vector<T, N> &v)
    {
        for (size_t i = 0; i < N; i++)
            if (u[i] != v[i])
                return true;

        return false;
    }

    // Output operator.
    template <class T, int N>
    std::ostream &operator<<(std::ostream& out, const Vector<T, N> v)
    {
        out << to_string(v) << std::endl;
        return out;
    }

    // The sum of two vectors.
    template <class T, int N>
    Vector<T, N> operator+(const Vector<T, N> &u, const Vector<T, N> &v)
    {
        Vector<T, N> vec = Vector<T, N>();
        for (size_t i = 0; i < N; ++i)
        {
            vec[i] = u[i] + v[i];
        }
        return vec;
    }

    // The negation of a vector.
    template <class T, int N>
    Vector<T, N> operator-(const Vector<T, N> &v)
    {
        Vector<T, N> vec = Vector<T, N>();
        for (size_t i = 0; i < N; ++i)
        {
            vec[i] = v[i] * (-1);
        }
        return vec;
    }

    // The subtraction of two vectors.
    template <class T, int N>
    Vector<T, N> operator-(const Vector<T, N> &u, const Vector<T, N> &v)
    {

        Vector<T, N> vec = Vector<T, N>();
        for (size_t i = 0; i < N; ++i)
        {
            vec[i] = u[i] - v[i];
        }
        return vec;
    }

    // The product of a scalar and a vector.
    template <class T1, class T2, int N>
    Vector<T2, N> operator*(const T2 &s, const Vector<T1, N> &v)
    {
        Vector<T2, N> vec = Vector<T2, N>();
        for (size_t i = 0; i < N; ++i)
        {
            vec[i] = v[i] * s;
        }
        return vec;
    }

    // The product of a vector and a scalar.
    template <class T1, class T2, int N>
    Vector<T2, N> operator*(const Vector<T1, N> &v, const T2 &s)
    {
        return s * v;
    }

    // The product of two vectors.
    template <class T, int N>
    Vector<T, N> operator*(const Vector<T, N> &u, const Vector<T, N> &v)
    {   
        Vector<T,N> result = Vector<T,N>();
        for(int i = 0; i < N; i++){
            result[i] = u[i]*v[i];
        }
        return result;
        
    }

    // The division of a vector by a scalar (same as the multiplication by 1/s).
    template <class T1, class T2, int N>
    Vector<T1, N> operator/(const Vector<T1, N> &v, const T2 &s)
    {
        return (1 / s) * v;
    }

    // The square of the norm (magnitude) of the vector.
    template <class T, int N>
    float sq_norm(const Vector<T, N> &v)
    {
        float n = norm(v);
        return n * n;
    }

    // A string representation of a vector.
    template <class T, int N>
    std::string to_string(const Vector<T, N> &v)
    {
        std::stringstream ss;
        //ss.precision(6);
        ss << "(";
        for (size_t i = 0; i < N - 1; ++i)
        {
            ss << std::fixed << v[i] << ", ";
        }
        ss << std::fixed << v[N - 1] << ")";
        return ss.str();
    }

    // The vector normalized.
    template <class T, int N>
    Vector<T, N> unit_vector(const Vector<T, N> &v)
    {
        return v / norm(v);
    }
}