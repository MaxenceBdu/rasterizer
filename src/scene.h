#include "shape.h"
#include <string>
#include "window.h"
#include <assert.h>

namespace aline{

    class Scene{
        std::vector<Shape> shapes;
        minwin::Window window;
    public:
        Scene(){
            shapes = std::vector<Shape>();
        }

        // Adds a shape to the scene.
        void add_shape( const Shape & s){
            shapes.push_back(s);
        }
        
        // Opens a MinWin window and sets its parameters (for instance, title and size).
        void initialise(){
            window = minwin::Window();
            window.set_title("Test window");
            window.set_width(500ul);
            window.set_height(300ul);
            window.display();
            assert(window.open());
        }
        
        /* Draws the shapes previously added to the scene on the window surface1 and process
        users inputs (e.g., click on ‘X’ to quit). It must keep the window open and showing
        the shapes until the user decides to quit the application, either by clicking on the
        button ‘X’ or via some keyboard command (e.g., ‘ESC’ or ‘Q’). It must also permit
        the user to change the “view mode” of the scene. Via some keyboard command, the
        user can change from a “wireframe” mode of the scene to a “solid” and a “shaded”
        (optional bonus feature) mode. */
        void run(){
            while(window.open()){
                window.clear();
                
                window.render_fps_info();
            }
        }
        
        /*Closes the MinWin window and frees eventual allocated memory. (For example, if
        your function add_shape() creates a list of shapes, you must clear the list.)*/
        void shutdown(){
            window.close();
            assert(!window.open());
        }
    };


}