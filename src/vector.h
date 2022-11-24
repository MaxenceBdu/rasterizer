#include <iostream>
#include <sstream>
#include <cmath>

namespace aline {

    template<class T, int N> class Vector{
    public:
        size_t size;
        T* elmts;

        // default constructor
        Vector(){
            size = N;
            elmts = new T[size];
            for(size_t i = 0; i < size; i++)
                elmts[i] = 0;
        }

        // Initialize with list of values
        Vector(std::initializer_list<T> l){
            if(l.size() > N)
                throw std::runtime_error("Initializer list is too long");
            size = l.size();
            elmts = new T[size];
            size_t i = 0;
            for(auto& item : l){ // fill with values of l
                elmts[i] = item;
                ++i;
            }
            for(size_t j = i; j < size; ++j){ // fill with zeros if l.size() < N
                elmts[j] = 0;
            }
        }

        // Copy constructor
        Vector(const Vector<T,N> &v ){
            size = v.size;
            elmts = new T[size];
            for(size_t i = 0; i < size; i++){
                elmts[i] = v.elmts[i];
            }
        }

        T& at(size_t i) const {
            if(i >= size)
                throw std::runtime_error("Indice invalide");

            return elmts[i];
        }

        T operator[](size_t i) const {
            return elmts[i];
        }
        
        T& operator[](size_t i) {
            return elmts[i];
        }

        Vector<T,N>& operator+=( const Vector<T,N> &v ){
            if(v.size != size){
                throw std::runtime_error("Incompatible sizes");
            }

            for(size_t i = 0; i < size; i++){
                elmts[i] = elmts[i] + v[i];
            }

            return *this;
        }


    };

    // The cross product of two vectors. Uses only the first 3 elements (zero the others in
    // the result). Throws runtime_error if the vectors have less than 3 elements.
    template<class T, int N> Vector<T,N> cross( const Vector<T,N> &u, const Vector<T,N> &v );
    
    // The dot product of two vectors.
    template<class T, int N> T dot( const Vector<T,N> &u, const Vector<T,N> &v);

    // Tests if the vector contains NAN (not a number) values. (It is sometimes useful when
    // the result of a computation does not exist, e.g. division by zero).
    template<class T, int N> bool isnan( const Vector<T,N> &v);

    // Tests if the vector is a unit vector.
    template<class T, int N> bool is_unit( const Vector<T,N> &v );
    
    // Tests if two vectors contain nearly equal values. Two values are nearly equal when
    // they are very close, with respect to their magnitudes. For example, 1.0000001 can be
    // considered nearly equal to 1, whereas 1.234 is not nearly equal to 1.242. However,
    // because of their magnitude, 67329.234 can be considered nearly equal to 67329.242.1
    template<class T, int N> bool nearly_equal( const Vector<T,N> &u, const Vector<T,N> &v );

    // The norm (magnitude) of the vector
    template<class T, int N> float norm(const Vector<T,N> &v);
 
    // Tests if two vectors contain the same values.
    template<class T, int N> bool operator==( const Vector<T,N> &u, const Vector<T,N> &v);
    
    // Test if two vectors contain different values.
    template<class T, int N> bool operator!=( const Vector<T,N> & u, const Vector<T,N> & v);

    // Output operator.
    template<class T, int N> std::ostream& operator<<( std::ostream &out, const Vector<T,N> v );

    // The sum of two vectors.
    template<class T, int N> Vector<T,N> operator+( const Vector<T,N> &u, const Vector<T,N> &v );

    // The negation of a vector.
    template<class T, int N> Vector<T,N> operator-( const Vector<T,N> &v );

    // The subtraction of two vectors.
    template<class T, int N> Vector<T,N> operator-( const Vector<T,N> &u, const Vector<T,N> &v );

    // The product of a scalar and a vector.
    template<class T1, class T2, int N> Vector<T1,N> operator*( const T2 &s, const Vector<T1,N> & v);

    // The product of a vector and a scalar.
    template<class T1, class T2, int N> Vector<T1,N> operator*( const Vector<T1,N> &v, const T2 &s );

    // The dot product of two vectors.
    template<class T, int N> Vector<T,N> operator*( const Vector<T,N> &u, const Vector<T,N> &v );

    // The division of a vector by a scalar (same as the multiplication by 1/s).
    template<class T1, class T2, int N> Vector<T1,N> operator/( const Vector<T1,N> & v, const T2 & s );

    // The square of the norm (magnitude) of the vector.
    template<class T, int N> float sq_norm(const Vector<T,N> &v);
    

    // A string representation of a vector.
    template<class T, int N> std::string to_string( const Vector<T,N> &v );

    // The vector normalized.
    template<class T, int N> Vector<T,N> unit_vector( const Vector<T,N> & v );
}