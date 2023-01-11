## Description

This program was made during **Mathematical Tools for game programming** course in Artois University.  
It is called *rasterizer* and renders a 3D object in a window generated thanks to the *minwin* library.  
The *minwin* libray is the work of Mr Tiago De Lima, professor and researcher at CRIL and Artois University.
This *rasterizer* was made and tested under Linux only.

## Steps to execute the program

In a terminal :
- Go in the rasterizer folder
- export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:< path of rasterizer folder >/rasterizer/minwin/bin (you can use the ~)
- [make clean]
- make all
- ./bin/test_scene assets/teapot.obj

## Not implemented :
- Clipping
- Back face culling
- Hidden surface removal
  
## Known bugs :
- Object may disappear when the camera rotates around z axis
