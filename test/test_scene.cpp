#include "scene.h"
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{

  Scene s = Scene();
  s.initialise();

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
        std::vector<uint> ids;
        std::istringstream iss(str);
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
        std::vector<aline::real> values;
        std::istringstream iss(str);
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
    
    Shape shape(argv[i], verts, faces);
    Object o(shape, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0});
    
    s.add_object(o);
  }

  s.run();

  return 0;
}