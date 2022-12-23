#include "color.h"
#include <string>
#include <vector>
#include "vector.h"


class Vertex {
    aline::Vec3r vec;
    aline::real h;
public:
    Vertex( const aline::Vec3r & c, aline::real h ) : vec(c), h(h){
    }

    inline aline::Vec3r get_vec() const {
        return vec;
    }
};

class Face{
    uint v0, v1, v2;
    minwin::Color color;
public:
    Face( uint v0, uint v1, uint v2, const minwin::Color & color ) :v0(v0), v1(v1), v2(v2), color(color) {
        
    }

    inline uint get_v0() const {
        return v0;
    }

    inline uint get_v1() const {
        return v1;
    }

    inline uint get_v2() const {
        return v2;
    }

    inline minwin::Color get_color() const {
        return color;
    }
};

class Shape{
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
public: 
    Shape( const std::string & name, const std::vector<Vertex> & vertices, const std::vector<Face> & faces ) : name(name), vertices(vertices), faces(faces){
    }

    // Returns the name of the face.
    inline std::string get_name() const{
        return name;
    }
    // Returns the list of vertices.
    inline std::vector<Vertex> get_vertices() const{
        return vertices;
    }
    
    // Returns the list of faces
    inline std::vector<Face> get_faces() const{
        return faces;
    }
    
};

class Object {
    Shape& shape;
    aline::Vec3r& translation;
    aline::Vec3r& rotation;
    aline::Vec3r& scale;

    Object(){}
public:
    Object( const Shape & shape, const Vec3r & translation, const Vec3r & rotation, const Vec3r & scale ){
        this->shape = shape;
        this->translation = translation;
        this->rotation = rotation;
        this->scale = scale;
    }

    aline::Matrix<aline::real,4ul,3ul> transform(){

    }

    // The list of vertices of an object.
    std::vector<Vertex> get_vertices(){
        return shape.get_vertices();
    }

    // The list of faces of an object
    std::vector<Face> get_faces(){
        return shape.get_faces();
    }
};
