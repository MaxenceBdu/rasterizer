#include <iostream>

namespace aline {
    template<class T, int N> class Vector{
    public:
        T* elmts;

        // default constructor
        Vector(){
            elmts = new T[N];
            for(int i = 0; i < N; i++)
                elmts[i] = 0;
        }

        // Initialize with list of values
        Vector(std::initializer_list<T> l){
            if(l.size() > N)
                throw std::runtime_error("Initializer list is too long");

            elmts = new T[N];
            int i = 0;
            for(auto& item : l){ // fill with values of l
                elmts[i] = item;
                i++;
            }
            for(i; i < N; i++){ // fill with zeros if l.size() < N
                elmts[i] = 0;
            }
        }

        // Copy constructor
        Vector(const Vector<T,N> &v ){
            elmts = new T[v.N];
            for(int i = 0; i < v.N; i++){
                elmts[i] = v.elmts[i];
            }
        }

        T& at(size_t i) const {
            if(i >= N)
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
            if(v.N != N){
                throw std::runtime_error("Incompatible sizes");
            }

            for(int i = 0; i < N; i++){
                elmts[i] = elmts[i] + v[i];
            }

            return *this;
        }

        T norm(){}
    };

    // The cross product of two vectors. Uses only the first 3 elements (zero the others in
    // the result). Throws runtime_error if the vectors have less than 3 elements.
    template<class T, int N> Vector<T,N> cross( const Vector<T,N> &u, const Vector<T,N> &v ){
        if(u.N < 3)
            throw std::runtime_error("Vectors size is inferior to 3");
        return Vector<T,3>({u[1]*v[2] - u[2]*v[1], u[2]*v[0] - u[0]*v[2], u[0]*v[1] - u[1]*v[0]});
    }
    
    // The dot product of two vectors.
    template<class T, int N> T dot( const Vector<T,N> &u, const Vector<T,N> &v){
        T result;
        for(int i = 0; i < u.N; i++){
            result += u[1]*v[i];
        }
    }

    // Tests if the vector contains NAN (not a number) values. (It is sometimes useful when
    // the result of a computation does not exist, e.g. division by zero).
    template<class T, int N> bool isnan( const Vector<T,N> &v){
        for(int i = 0; i < v.N; i++){
            if(!std::isdigit(v[i]))
                return false;
        }
        return true;
    }

    // Tests if the vector is a unit vector.
    template<class T, int N> bool is_unit( const Vector<T,N> &v ){
        return v.norm() == 1;
    }
    
    // Tests if two vectors contain nearly equal values. Two values are nearly equal when
    // they are very close, with respect to their magnitudes. For example, 1.0000001 can be
    // considered nearly equal to 1, whereas 1.234 is not nearly equal to 1.242. However,
    // because of their magnitude, 67329.234 can be considered nearly equal to 67329.242.1
    template<class T, int N> bool nearly_equal( const Vector<T,N> &, const Vector<T,N> &v ){

    }
   
    // The norm (magnitude) of the vector.
    template<class T, int N> T Vector<T,N>::norm(){
        return N;
    }

    // Tests if two vectors contain the same values.
    template<class T, int N> bool operator==( const Vector<T,N> &u, const Vector<T,N> &v){
        for(int i = 0; i < u.N; i++)
            if(u[i] != v[i]) return false;
        
        return true;
    }
    
    // Test if two vectors contain different values.
    template<class T, int N> bool operator!=( const Vector<T,N> & u, const Vector<T,N> & v){
        for(int i = 0; i < u.N; i++)
            if(u[i] != v[i]) return true;
        
        return false;
    }

    // Output operator.
    template<class T, int N> std::ostream& operator<<( std::ostream &out, const Vector<T,N> v ){
        for(int i = 0; i < v.N; i++){
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
}