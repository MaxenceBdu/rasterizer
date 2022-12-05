#include "shape.h"
#include <string>
#include "window.h"
#include <assert.h>

#define VIEWPORT_WIDTH 2.0
#define WINDOW_WIDTH 1366.0
#define WINDOW_HEIGHT 768.0
#define VIEWPORT_HEIGHT (WINDOW_HEIGHT / WINDOW_WIDTH * VIEWPORT_WIDTH)

namespace aline
{
  class Scene
  {
    std::vector<Shape> shapes;
    minwin::Window window;
    bool running;
    minwin::Text text;

  public:
    Scene()
    {
      shapes = std::vector<Shape>();
      text.set_pos(10, 10);
      text.set_string("Press ESC to quit.");
      text.set_color(minwin::RED);
      running = true;
    }

    // Adds a shape to the scene.
    void add_shape(const Shape &s)
    {
      shapes.push_back(s);
    }

    // Opens a MinWin window and sets its parameters (for instance, title and size).
    void initialise()
    {
      window = minwin::Window();
      window.set_title("Test window");
      window.set_width(WINDOW_WIDTH);
      window.set_height(WINDOW_HEIGHT);
      window.register_quit_behavior( new QuitButtonBehavior( *this ) );
      window.register_key_behavior(minwin::KEY_ESCAPE, new QuitKeyBehavior(*this));

      // load font
      if( not window.load_font( "fonts/FreeMonoBold.ttf", 16u ) )
      {
        std::cerr << "Couldn't load font.\n";
      }

      // open window
      if (not window.open())
      {
        std::cerr << "Couldn't open window.\n";
        return;
      }
    }

    /* Draws the shapes previously added to the scene on the window surface1 and process
    users inputs (e.g., click on ‘X’ to quit). It must keep the window open and showing
    the shapes until the user decides to quit the application, either by clicking on the
    button ‘X’ or via some keyboard command (e.g., ‘ESC’ or ‘Q’). It must also permit
    the user to change the “view mode” of the scene. Via some keyboard command, the
    user can change from a “wireframe” mode of the scene to a “solid” and a “shaded”
    (optional bonus feature) mode. */
    void run()
    {
      while (this->running)
      {
        // process keyboard inputs, etc.
        window.process_input();

        // clear window
        window.clear();

        // draw text
        window.render_text(text);

        for (Shape s : shapes)
        {
          std::vector<Face> faces = s.get_faces();
          std::vector<Vertex> verts = s.get_vertices();
          for (Face f : faces)
          {
            window.set_draw_color(f.get_color());
            draw_line(verts[f.get_v0()].get_vec(), verts[f.get_v1()].get_vec());
            draw_line(verts[f.get_v1()].get_vec(), verts[f.get_v2()].get_vec());
            draw_line(verts[f.get_v2()].get_vec(), verts[f.get_v0()].get_vec());
          }
        }

        // display elements drawn so far
        window.display();
      }
      window.close();
    }

    /*Closes the MinWin window and frees eventual allocated memory. (For example, if
    your function add_shape() creates a list of shapes, you must clear the list.)*/
    void shutdown()
    {
      this->running = false;
    }

    // Converts viewport coordinates of a point to canvas coordinates.
    Vec2r viewport_to_canvas(const Vec2r &point) const
    {
      if(VIEWPORT_HEIGHT <= 0)
        throw std::runtime_error("Viewport height <= 0");

      return Vec2r({point[0] * WINDOW_WIDTH / VIEWPORT_WIDTH, point[1] * WINDOW_HEIGHT / VIEWPORT_HEIGHT});
    }

    // Converts canvas coordinates of a point to window (screen) coordinates.
    Vec2i canvas_to_window(const Vec2r &point) const
    {
      return Vec2i({WINDOW_WIDTH / 2 + point[0], WINDOW_HEIGHT / 2 - point[1]});
    }

    // Draws a line from v0 to v1 using the current drawing color.
    // I use Bresenham's algorithm
    void draw_line(const Vec2r &v0, const Vec2r &v1) const
    {
      Vec2i _v0 = canvas_to_window(viewport_to_canvas(v0));
      Vec2i _v1 = canvas_to_window(viewport_to_canvas(v1));

      int dx = _v1[0] - _v0[0];
      int dy = _v1[1] - _v0[1];
      int D = 2*dy - dx;
      int y = _v0[1];

      for(int x = _v0[0]; x <= _v1[0]; ++x){
          window.put_pixel(x, y);
          if(D > 0){
              y = y + 1;
              D = D - 2*dx;
          }
          D = D + 2*dy;
      }
    }

  private:
    class QuitKeyBehavior : public minwin::IKeyBehavior{
    public:
      QuitKeyBehavior(Scene &scene) : owner{scene} {}
      void on_press() const
      {
        this->owner.shutdown();
      };
      void on_release() const {};

    private:
      Scene &owner;
    };

    class QuitButtonBehavior : public minwin::IButtonBehavior
    {
      public:
        QuitButtonBehavior( Scene & s ) : owner { s } {}
        void on_click() const {
          this->owner.running = false;
        }
      private:
        Scene & owner;
    };
  };

}