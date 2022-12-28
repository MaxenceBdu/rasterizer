#include "color.h"
#include <string>
#include <vector>
#include "matrix.h"

#define PI_180 M_PI / 180

class Vertex
{
  aline::Vec3r vec;
  aline::real h;

public:
  Vertex(const aline::Vec3r &c, aline::real h) : vec(c), h(h)
  {
  }

  inline aline::Vec3r get_vec() const
  {
    return vec;
  }
};

class Face
{
  uint v0, v1, v2;
  minwin::Color color;

public:
  Face(uint v0, uint v1, uint v2, const minwin::Color &color) : v0(v0), v1(v1), v2(v2), color(color)
  {
  }

  inline uint get_v0() const
  {
    return v0;
  }

  inline uint get_v1() const
  {
    return v1;
  }

  inline uint get_v2() const
  {
    return v2;
  }

  inline minwin::Color get_color() const
  {
    return color;
  }
};

class Shape
{
  std::string name;
  std::vector<Vertex> vertices;
  std::vector<Face> faces;

public:
  Shape(const std::string &name, const std::vector<Vertex> &vertices, const std::vector<Face> &faces) : name(name)
  {
    this->vertices = std::vector<Vertex>(vertices);
    this->faces = std::vector<Face>(faces);
  }

  Shape(const Shape& shape)
  {
    this->name = shape.get_name();
    this->vertices = std::vector<Vertex>(shape.get_vertices());
    this->faces = std::vector<Face>(shape.get_faces());
  }

  // Returns the name of the face.
  inline std::string get_name() const
  {
    return name;
  }
  // Returns the list of vertices.
  inline std::vector<Vertex> get_vertices() const
  {
    return vertices;
  }

  // Returns the list of faces
  inline std::vector<Face> get_faces() const
  {
    return faces;
  }
};

aline::Vec4r w({0.0,0.0,0.0,1.0});

class Object
{
  const Shape* shape;
  aline::Vec3r translation;
  aline::Vec3r rotation;
  aline::Vec3r scale;

public:
  Object(const Shape* shape, const aline::Vec3r &translation, const aline::Vec3r &rotation, const aline::Vec3r &scale) : shape(shape)
  {
    this->shape = shape;
    this->translation = aline::Vec3r(translation);
    this->rotation = aline::Vec3r(rotation);
    this->scale = aline::Vec3r(scale);

    //std::cout << to_string(transform()) << std::endl;
  }

  /*
    Returns the transform matrix

    Transform matrix = translation * rotation * scaling
  */
  aline::Mat44r transform(){

    aline::Mat44r translation_matrix({
      {1.0, 0.0, 0.0, translation[0]},
      {0.0, 1.0, 0.0, translation[1]},
      {0.0, 0.0, 1.0, translation[2]},
      {0.0, 0.0, 0.0, 1.0}
    });

    aline::real alpha = degrees_to_radians(rotation[0]);
    aline::real beta = degrees_to_radians(rotation[1]);
    aline::real gamma = degrees_to_radians(rotation[2]);
    aline::Mat44r rotation_matrix({
      {cos(beta)*cos(gamma), cos(beta)*sin(gamma), -sin(beta), 0.0},
      {sin(alpha)*sin(beta)*cos(gamma)-cos(alpha)*sin(gamma), sin(alpha)*sin(beta)*sin(gamma)+cos(alpha)*cos(gamma), sin(alpha)*cos(beta), 0.0},
      {cos(alpha)*sin(beta)*cos(gamma)+sin(alpha)*sin(gamma), cos(alpha)*sin(beta)*sin(gamma)-sin(alpha)*cos(gamma), cos(alpha)*cos(beta), 0.0},
      {0.0, 0.0, 0.0, 1.0}
    });

    aline::Mat44r scale_matrix({
      {scale[0], 0.0, 0.0, 0.0},
      {0.0, scale[1], 0.0, 0.0},
      {0.0, 0.0, scale[2], 0.0},
      {0.0, 0.0, 0.0, 1.0}
    });
    return (translation_matrix*rotation_matrix*scale_matrix);
  }

  Shape get_shape() const{
    return *shape;
  }

  //  The list of vertices of an object.
  std::vector<Vertex> get_vertices()
  {
    return shape->get_vertices();
  }

  // The list of faces of an object
  std::vector<Face> get_faces()
  {
    return shape->get_faces();
  }

  void scale_up(){
    scale = scale + aline::Vec3r({1,1,1});
  }

  void scale_down(){
    scale = scale + aline::Vec3r({-1,-1,-1});
  }

  aline::real degrees_to_radians(aline::real x){
    return x * PI_180;
  }
};
