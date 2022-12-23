#include "color.h"
#include <string>
#include <vector>
#include "vector.h"

namespace aline {
    class Vertex {
        Vec2r vec;
        real h;
    public:
        Vertex( const Vec2r & c, real h ){
            vec = c;
            this->h = h;
        }

        inline Vec2r get_vec() const {
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
}