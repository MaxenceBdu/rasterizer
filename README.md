- I modified the Makefile to include the minwin library. It will look for the minwin folder in the home directory (~) so make sure to have the minwin folder at the right place.  

- To see my latest work : 
  - make clean
  - make all
  - ./bin/test_scene assets/teapot.obj

I didn't manage to implement clipping, back face culling and hidden surface removal for the moment.  
Information about camera's movement are displayed in the window.