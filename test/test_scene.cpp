#include "scene.h"

using namespace std;

int main(int argc, char* argv[]){
    
    Scene s = Scene();
    s.initialise();

    if(argc <= 1)
        throw runtime_error("No filename given");

    for(int i = 1; i < argc; ++i){
        s.load_obj_file(argv[i]);
    }
    
    s.run();

    return 0;
}