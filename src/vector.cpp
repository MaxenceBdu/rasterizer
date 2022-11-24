#include "vector.h"
#include <float.h>

float epsilon = FLT_EPSILON;

using namespace aline;

// The cross product of two vectors. Uses only the first 3 elements (zero the others in
// the result). Throws runtime_error if the vectors have less than 3 elements.
template<class T, int N> Vector<T,N> cross( const Vector<T,N> &u, const Vector<T,N> &v ){
    if(u.size < 3 || v.size < 3 || u.size != v.size)
        throw std::runtime_error("Vectors size is inferior to 3");
    Vector<T,N> vec = Vector<T,N>({u[1]*v[2] - u[2]*v[1], u[2]*v[0] - u[0]*v[2], u[0]*v[1] - u[1]*v[0]});
    for(size_t i = 3; i < u.size; ++i){
        vec[i]=0;
    }
    return vec;
}

// The dot product of two vectors.
template<class T, int N> T dot( const Vector<T,N> &u, const Vector<T,N> &v){
    T result;
    for(size_t i = 0; i < u.size; i++){
        result += u[1]*v[i];
    }
    return result;
}

// Tests if the vector contains NAN (not a number) values. (It is sometimes useful when
// the result of a computation does not exist, e.g. division by zero).
template<class T, int N> bool isnan( const Vector<T,N> &v){
    for(size_t i = 0; i < v.size; i++){
        if(std::isnan(v[i]))
            return true;
    }
    return false;
}

// Tests if the vector is a unit vector.
template<class T, int N> bool is_unit( const Vector<T,N> &v ){
    return v.norm() == 1;
}

// Tests if two vectors contain nearly equal values. Two values are nearly equal when
// they are very close, with respect to their magnitudes. For example, 1.0000001 can be
// considered nearly equal to 1, whereas 1.234 is not nearly equal to 1.242. However,
// because of their magnitude, 67329.234 can be considered nearly equal to 67329.242.1
template<class T, int N> bool nearly_equal( const Vector<T,N> &u, const Vector<T,N> &v ){

    if(u.size != v.size)
        return false;

    for(size_t i = 0; i < u.size; ++i){
        float diff = fabs(u[i] - v[i]);
        float A = fabs(u[i]);
        float B = fabs(v[i]);

        // Find the largest
        float largest = (B > A) ? B : A;

        if (diff > largest * epsilon)
            return false;
    }
    return true;
}

// The norm (magnitude) of the vector.
template<class T, int N> float norm(const Vector<T,N> &v){
    return sqrt(dot(v, v));
}

// Tests if two vectors contain the same values.
template<class T, int N> bool operator==( const Vector<T,N> &u, const Vector<T,N> &v){
    for(size_t i = 0; i < u.size; i++)
        if(u[i] != v[i]) return false;
    
    return true;
}

// Test if two vectors contain different values.
template<class T, int N> bool operator!=( const Vector<T,N> & u, const Vector<T,N> & v){
    for(size_t i = 0; i < u.size; i++)
        if(u[i] != v[i]) return true;
    
    return false;
}

// Output operator.
template<class T, int N> std::ostream& operator<<( std::ostream &out, const Vector<T,N> v ){
    for(size_t i = 0; i < v.size; i++){
        out << v[i];
    }
    return out;
}

// The sum of two vectors.
template<class T, int N> Vector<T,N> operator+( const Vector<T,N> &u, const Vector<T,N> &v ){
    return Vector<T,3>({u[0]+v[0], u[1]+v[1], u[2]+v[2]});
}

// The negation of a vector.
template<class T, int N> Vector<T,N> operator-( const Vector<T,N> &v ){
    return Vector<T,3>({-v[0], -v[1], -v[2]});
}

// The subtraction of two vectors.
template<class T, int N> Vector<T,N> operator-( const Vector<T,N> &u, const Vector<T,N> &v ){
    if(u.size < 3 || v.size < 3 || u.size != v.size)
        throw std::runtime_error("Vectors size is inferior to 3");
    Vector<T,N> vec = Vector<T,N>({u[0]-v[0],u[1]-v[1],u[2]-v[2]});
    for(size_t i = 3; i < u.size; ++i){
        vec[i]=0;
    }
    return vec;
}

// The product of a scalar and a vector.
template<class T1, class T2, int N> Vector<T1,N> operator*( const T2 &s, const Vector<T1,N> & v){
    return Vector<T1,3>({v[0]*s,v[1]*s,v[2]*s});
}

// The product of a vector and a scalar.
template<class T1, class T2, int N> Vector<T1,N> operator*( const Vector<T1,N> &v, const T2 &s ){
    return s*v;
}

// The dot product of two vectors.
template<class T, int N> Vector<T,N> operator*( const Vector<T,N> &u, const Vector<T,N> &v ){
    return dot(u,v);
}

// The division of a vector by a scalar (same as the multiplication by 1/s).
template<class T1, class T2, int N> Vector<T1,N> operator/( const Vector<T1,N> & v, const T2 & s ){
    return 1/s*v;
}

// The square of the norm (magnitude) of the vector.
template<class T, int N> float sq_norm(const Vector<T,N> &v){
    return norm(v)*norm(v);
}

// A string representation of a vector.
template<class T, int N> std::string to_string( const Vector<T,N> &v ){
    std::stringstream ss;
    for(size_t i = 0; i < v.size; ++i){
        ss << v[i];
    }
    return ss.str();
}

// The vector normalized.
template<class T, int N> Vector<T,N> unit_vector( const Vector<T,N> & v ){
    return v/v.norm();
}