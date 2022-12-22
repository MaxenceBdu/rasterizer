#include "shape.h"
#include "matrix.h"

class Object
{

    const Shape * shape;
    aline::Vec3r translation;
    aline::Vec3r rotation;
    aline::Vec3r scale;

public:
    Object(const Shape& shape, const aline::Vec3r &translation, const aline::Vec3r &rotation, const aline::Vec3r &scale) : shape(&shape), translation(translation), rotation(rotation), scale(scale)
    {
    }

    aline::Matrix<aline::real,3,3> transform(){
        return aline::Matrix<aline::real,3,3>({translation, rotation, scale});
    }

    // The list of vertices of an object.
    inline std::vector<Vertex> get_vertices(){
        return shape->get_vertices();
    }

    // The list of faces of an object
    inline std::vector<Face> get_faces(){
        return shape->get_faces();
    }
};