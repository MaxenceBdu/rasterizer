#include "object.h"
#include <string>
#include "window.h"
#include <assert.h>

#define CANVAS_DIM 700
#define WINDOW_WIDTH 1366.0
#define WINDOW_HEIGHT 768.0
#define VIEWPORT_WIDTH 2.0
#define VIEWPORT_HEIGHT (CANVAS_DIM / CANVAS_DIM * VIEWPORT_WIDTH)

// X_DIFF and Y_DIFF are useful to center the drawing
#define X_DIFF std::round((WINDOW_WIDTH - CANVAS_DIM) / 2)
#define Y_DIFF std::round((WINDOW_HEIGHT - CANVAS_DIM) / 2)

enum DrawMode
{
  wireframe,
  solid
};

class Scene
{
  std::vector<Object> objects;
  minwin::Window window;
  bool running;
  minwin::Text text1;
  minwin::Text text2;
  DrawMode draw_mode;

public:
  Scene()
  {
    objects = std::vector<Object>();
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
  void add_object(const Object &s)
  {
    objects.push_back(s);
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

  /* Draws the objects previously added to the scene on the window surface1 and process
  users inputs (e.g., click on ‘X’ to quit). It must keep the window open and showing
  the objects until the user decides to quit the application, either by clicking on the
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

      for (Object o : objects)
      {
        std::vector<Face> faces = o.get_faces();
        std::vector<Vertex> verts = o.get_vertices();

        for (Face f : faces)
        {
          // make homogeneous coordinates and perspective projection
          aline::Vec3r _v0 = verts[f.get_v0()].get_vec();
          aline::Vec3r _v1 = verts[f.get_v1()].get_vec();
          aline::Vec3r _v2 = verts[f.get_v2()].get_vec();
 
          aline::Vec2r v0 = perspective_projection(aline::Vec4r({_v0[0], _v0[1], _v0[2], 1.0})*o.transform(), 1.0);
          aline::Vec2r v1 = perspective_projection(aline::Vec4r({_v1[0], _v1[1], _v1[2], 1.0})*o.transform(), 1.0);
          aline::Vec2r v2 = perspective_projection(aline::Vec4r({_v2[0], _v2[1], _v2[2], 1.0})*o.transform(), 1.0);

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
            draw_filled_triangle(v0, v1, v2);
            window.set_draw_color(minwin::BLACK);
            draw_wireframe_triangle(v0, v1, v2);
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
  your function add_shape() creates a list of objects, you must clear the list.)*/
  void shutdown()
  {
    this->running = false;
  }

  // Converts viewport coordinates of a point to canvas coordinates.
  aline::Vec2r viewport_to_canvas(const aline::Vec2r &point) const
  {
    if (VIEWPORT_HEIGHT <= 0)
      throw std::runtime_error("Viewport height <= 0");

    return aline::Vec2r({point[0] * CANVAS_DIM / VIEWPORT_WIDTH, point[1] * CANVAS_DIM / VIEWPORT_HEIGHT});
  }

  // Converts canvas coordinates of a point to window (screen) coordinates.
  aline::Vec2i canvas_to_window(const aline::Vec2r &point) const
  {
    return aline::Vec2i({(int)std::round(CANVAS_DIM / 2 + point[0]), (int)std::round(CANVAS_DIM / 2 - point[1])});
  }

  // Draws a line from v0 to v1 using the current drawing color.
  // I use Bresenham's algorithm (Wikipedia)
  void draw_line(const aline::Vec2r &v0, const aline::Vec2r &v1) const
  {
    aline::Vec2i _v0 = canvas_to_window(viewport_to_canvas(v0));
    aline::Vec2i _v1 = canvas_to_window(viewport_to_canvas(v1));

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

  void draw_wireframe_triangle(const aline::Vec2r &v0, const aline::Vec2r &v1, const aline::Vec2r &v2) const
  {
    draw_line(v0, v1);
    draw_line(v1, v2);
    draw_line(v2, v0);
  }

  void draw_filled_triangle(const aline::Vec2r &v0, const aline::Vec2r &v1, const aline::Vec2r &v2) const
  {
    aline::Vec2i _v0 = canvas_to_window(viewport_to_canvas(v0));
    aline::Vec2i _v1 = canvas_to_window(viewport_to_canvas(v1));
    aline::Vec2i _v2 = canvas_to_window(viewport_to_canvas(v2));

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

    std::vector<aline::real> x02 = interpolate(y0, x0, y2, x2);
    std::vector<aline::real> x01 = interpolate(y0, x0, y1, x1);
    std::vector<aline::real> x12 = interpolate(y1, x1, y2, x2);
    x01.pop_back();
    std::vector<aline::real> x012(x01);
    x012.insert(x012.end(), x12.begin(), x12.end());

    std::vector<aline::real> x_left, x_right;
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

  std::vector<aline::real> interpolate(int i0, aline::real d0, int i1, aline::real d1) const
  {
    if (i0 == i1)
      return std::vector<aline::real>(1, d0);

    aline::real a = (d0 - d1) / (i0 - i1);
    aline::real d = d0;
    std::vector<aline::real> values;
    for (int i = i0; i <= i1; ++i)
    {
      values.push_back(d);
      d = d + a;
    }
    return values;
  }

  // The perspective projection of the three dimentional vector v given in homogeneous
  // coordinates. The value of d is the distance from the camera to the viewport (also
  // called projection plane)
  aline::Vec2r perspective_projection(const aline::Vec4r &v, aline::real d)
  {
    // project in 3d
    aline::Vec3r projec_3d({v[0]/v[3], v[1]/v[3], v[2]/v[3]});


    // project in 2d
    if(projec_3d[2] == 0)
      return aline::Vec2r({0.0, 0.0});
    else
      return aline::Vec2r({projec_3d[0]/(projec_3d[2]), projec_3d[1]/(projec_3d[2])});
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