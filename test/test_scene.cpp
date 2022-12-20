#include "scene.h"

int main(int argc, char* argv[]){
    
    Scene s = Scene();
    s.initialise();

    if(argc <= 1)
        throw runtime_error("No filename given");

    for(int i = 1; i < argc; ++i){
        s.load_obj_file(argv[i]);
    }
    
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
        Face(2,3,4, minwin::MAGENTA)
    };
    
    Shape carre = Shape("carré", verts, faces);

    s.add_shape(carre);

    s.run();

    return 0;
}