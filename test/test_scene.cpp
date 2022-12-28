#include "scene.h"
#include <fstream>
#include <regex>

using namespace std;

int main(int argc, char *argv[])
{
  vector<Shape*> shapes;
  Scene s = Scene();
  s.initialise();

  // load object from file
  for (int i = 1; i < argc; ++i)
  {
    ifstream f(argv[i]);
    string str;
    vector<Vertex> verts;
    vector<Face> faces;
    while (f.good())
    {
      getline(f, str);

      if (str[0] == 'f')
      {
        vector<uint> ids;
        istringstream iss(str);
        char pass; // pass the 'v'
        iss >> pass;
        do
        {
          uint subs;
          iss >> subs;
          ids.push_back(subs);
        } while (iss);
        faces.push_back(Face(ids[0] - 1, ids[1] - 1, ids[2] - 1, minwin::WHITE));
      }
      else if (str[0] == 'v')
      {
        vector<aline::real> values;
        istringstream iss(str);
        char pass; // pass the 'v'
        iss >> pass;
        do
        {
          aline::real subs;
          iss >> subs;
          values.push_back(subs);
        } while (iss);
        verts.push_back(Vertex(aline::Vec3r({values[0], values[1], values[2]}), 1.0));
      }
    }

    shapes.push_back(new Shape(argv[i], verts, faces));

    aline::real z_translate = 3000.0;
    if(regex_match(argv[i], regex(".*tetrahedron.*"))){
      z_translate = 100.0;
    }

    Object o(shapes[shapes.size()-1], {0.0, 0.0, z_translate}, {0.0, 0.0, 0.0},  {1.0, 1.0, 1.0});
    s.add_object(o);
  }

  s.run();

  for(Shape* p: shapes){
    delete p;
  }
  return 0;
}