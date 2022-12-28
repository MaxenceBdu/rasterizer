#include "color.h"
#include <string>
#include <vector>
#include "matrix.h"

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
  Shape shape;
  aline::Mat44r translation;
  aline::Mat44r rotation;
  aline::Mat44r scale;

public:
  Object(const Shape& shape, const aline::Mat44r &translation, const aline::Mat44r &rotation, const aline::Mat44r &scale) : shape(shape)
  {
    this->shape = Shape(shape);
    this->translation = aline::Mat44r(translation);
    this->rotation = aline::Mat44r(rotation);
    this->scale = aline::Mat44r(scale);
  }

  /*
    Returns the transform matrix

    Transform matrix = translation * rotation * scaling
  */
  aline::Mat44r transform(){
    return translation*rotation*scale;
  }

  Shape get_shape(){
    return shape;
  }

  //  The list of vertices of an object.
  std::vector<Vertex> get_vertices()
  {
    return shape.get_vertices();
  }

  // The list of faces of an object
  std::vector<Face> get_faces()
  {
    return shape.get_faces();
  }
};
