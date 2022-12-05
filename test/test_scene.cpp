#include "scene.h"

using namespace aline;
int main(){
    
    Scene s = Scene();
    s.initialise();

    std::vector<Vertex> verts = { 
        Vertex({-1.0, 0.0 }, 1.0), 
        Vertex({-0.5,-0.75}, 1.0), 
        Vertex({ 0.5,-0.75}, 1.0), 
        Vertex({ 1.0, 0.0 }, 1.0), 
        Vertex({ 0.5, 0.75}, 1.0), 
        Vertex({-0.5, 0.75}, 1.0)
    };
    
    std::vector<Face> faces = { 
        Face(0,1,5, minwin::RED), 
        Face(1,2,4, minwin::BLUE), 
        Face(1,4,5, minwin::GREEN), 
        Face(2,3,4, minwin::YELLOW)
    };
    
    Shape carre = Shape("carré", verts, faces);

    s.add_shape(carre);

    s.run();

    return 0;
}