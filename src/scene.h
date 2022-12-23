#include "shape.h"
#include <string>
#include "window.h"
#include <assert.h>

#define CANVAS_DIM 768.0
#define WINDOW_WIDTH 1366.0
#define WINDOW_HEIGHT 768.0
#define VIEWPORT_WIDTH 2.0
#define VIEWPORT_HEIGHT (CANVAS_DIM / CANVAS_DIM * VIEWPORT_WIDTH)

// X_DIFF and Y_DIFF are useful to center the drawing
#define X_DIFF std::round((WINDOW_WIDTH - CANVAS_DIM) / 2)
#define Y_DIFF std::round((WINDOW_HEIGHT - CANVAS_DIM) / 2)

namespace aline
{
  enum DrawMode
  {
    wireframe,
    solid
  };

  class Scene
  {
    std::vector<Shape> shapes;
    minwin::Window window;
    bool running;
    minwin::Text text1;
    minwin::Text text2;
    DrawMode draw_mode;

  public:
    Scene()
    {
      shapes = std::vector<Shape>();
      text1.set_pos(10, 10);
      text1.set_string("Press ESC to quit");
      text1.set_color(minwin::RED);
      text2.set_pos(10, 30);
      text2.set_string("Press SPACE to change mode");
      text2.set_color(minwin::RED);
      running = true;
      draw_mode = wireframe;
    }

    DrawMode get_draw_mode()
    {
      return draw_mode;
    }

    void change_draw_mode()
    {
      switch (draw_mode)
      {
      case wireframe:
        draw_mode = solid;
        break;
      case solid:
        draw_mode = wireframe;
        break;
      default:
        break;
      }
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
      window.set_title("FMJ - Rasterizer");
      window.set_width(WINDOW_WIDTH);
      window.set_height(WINDOW_HEIGHT);
      window.register_quit_behavior(new QuitButtonBehavior(*this));
      window.register_key_behavior(minwin::KEY_ESCAPE, new QuitKeyBehavior(*this));
      window.register_key_behavior(minwin::KEY_SPACE, new ChangeDrawModeBehavior(*this));

      // load font
      if (not window.load_font("fonts/FreeMonoBold.ttf", 16u))
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
        window.render_text(text1);
        window.render_text(text2);

        for (Shape s : shapes)
        {
          std::vector<Face> faces = s.get_faces();
          std::vector<Vertex> verts = s.get_vertices();
          for (Face f : faces)
          {
            // set draw color with the color of current face
            Vec2r v0 = verts[f.get_v0()].get_vec();
            Vec2r v1 = verts[f.get_v1()].get_vec();
            Vec2r v2 = verts[f.get_v2()].get_vec();

            switch (draw_mode)
            {
            case wireframe:
              // draw wireframe triangle
              window.set_draw_color(minwin::WHITE);
              draw_wireframe_triangle(v0, v1, v2);
              break;
            case solid:
              // draw current face but filled
              window.set_draw_color(f.get_color());
              draw_wireframe_triangle(v0, v1, v2);
              draw_filled_triangle(v0, v1, v2);
              break;
            default:
              break;
            }
          }
        }

        // display elements drawn so far
        window.display();
      }
      window.close();
    }

  private:
    /*Closes the MinWin window and frees eventual allocated memory. (For example, if
    your function add_shape() creates a list of shapes, you must clear the list.)*/
    void shutdown()
    {
      this->running = false;
    }

    // Converts viewport coordinates of a point to canvas coordinates.
    Vec2r viewport_to_canvas(const Vec2r &point) const
    {
      if (VIEWPORT_HEIGHT <= 0)
        throw std::runtime_error("Viewport height <= 0");

      return Vec2r({point[0] * CANVAS_DIM / VIEWPORT_WIDTH, point[1] * CANVAS_DIM / VIEWPORT_HEIGHT});
    }

    // Converts canvas coordinates of a point to window (screen) coordinates.
    Vec2i canvas_to_window(const Vec2r &point) const
    {
      return Vec2i({(int)std::round(CANVAS_DIM / 2 + point[0]), (int)std::round(CANVAS_DIM / 2 - point[1])});
    }

    // Draws a line from v0 to v1 using the current drawing color.
    // I use Bresenham's algorithm
    void draw_line(const Vec2r &v0, const Vec2r &v1) const
    {
      Vec2i _v0 = canvas_to_window(viewport_to_canvas(v0));
      Vec2i _v1 = canvas_to_window(viewport_to_canvas(v1));

      int x0 = _v0[0], y0 = _v0[1];
      int x1 = _v1[0], y1 = _v1[1];
      int dx = abs(x1 - x0);
      int sx = x0 < x1 ? 1 : -1;
      int dy = -abs(y1 - y0);
      int sy = y0 < y1 ? 1 : -1;
      int error = dx + dy;

      while (true)
      {
        window.put_pixel(x0 + X_DIFF, y0 + Y_DIFF);
        if (x0 == x1 && y0 == y1)
          break;
        int e2 = 2 * error;
        if (e2 >= dy)
        {
          if (x0 == x1)
            break;
          error = error + dy;
          x0 = x0 + sx;
        }
        if (e2 <= dx)
        {
          if (y0 == y1)
            break;
          error = error + dx;
          y0 = y0 + sy;
        }
      }
    }

    void draw_wireframe_triangle(const Vec2r &v0, const Vec2r &v1, const Vec2r &v2) const
    {
      draw_line(v0, v1);
      draw_line(v1, v2);
      draw_line(v2, v0);
    }

    void draw_filled_triangle(const Vec2r &v0, const Vec2r &v1, const Vec2r &v2) const
    {
      Vec2i _v0 = canvas_to_window(viewport_to_canvas(v0));
      Vec2i _v1 = canvas_to_window(viewport_to_canvas(v1));
      Vec2i _v2 = canvas_to_window(viewport_to_canvas(v2));

      if (_v1[1] < _v0[1])
        std::swap(_v1, _v0);
      if (_v2[1] < _v0[1])
        std::swap(_v2, _v0);
      if (_v2[1] < _v1[1])
        std::swap(_v2, _v1);

      // refresh variables in case of swap
      int x0 = _v0[0], y0 = _v0[1];
      int x1 = _v1[0], y1 = _v1[1];
      int x2 = _v2[0], y2 = _v2[1];

      std::vector<real> x02 = interpolate(y0, x0, y2, x2);
      std::vector<real> x01 = interpolate(y0, x0, y1, x1);
      std::vector<real> x12 = interpolate(y1, x1, y2, x2);
      x01.pop_back();
      std::vector<real> x012(x01);
      x012.insert(x012.end(), x12.begin(), x12.end());

      std::vector<real> x_left, x_right;
      int m = (int)std::floor(x012.size() / 2);
      if (x02[m] < x012[m])
      {
        x_left = x02;
        x_right = x012;
      }
      else
      {
        x_left = x012;
        x_right = x02;
      }

      for (int y = y0; y <= y2; ++y)
        for (int x = (int)std::round(x_left[y - y0]); x <= (int)std::round(x_right[y - y0]); ++x)
          window.put_pixel(x + X_DIFF, y + Y_DIFF);
    }

    std::vector<real> interpolate(int i0, real d0, int i1, real d1) const
    {
      if (i0 == i1)
        return std::vector<real>(1, d0);

      real a = (d0 - d1) / (i0 - i1);
      real d = d0;
      std::vector<real> values;
      for (int i = i0; i <= i1; ++i)
      {
        values.push_back(d);
        d = d + a;
      }
      return values;
    }

    class QuitKeyBehavior : public minwin::IKeyBehavior
    {
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
      QuitButtonBehavior(Scene &s) : owner{s} {}
      void on_click() const
      {
        this->owner.running = false;
      }

    private:
      Scene &owner;
    };

    class ChangeDrawModeBehavior : public minwin::IKeyBehavior
    {
    public:
      ChangeDrawModeBehavior(Scene &s) : owner{s} {}
      void on_press() const {};
      void on_release() const
      {
        this->owner.change_draw_mode();
      }

    private:
      Scene &owner;
    };
  };

}