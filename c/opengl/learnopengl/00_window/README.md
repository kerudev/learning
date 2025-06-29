# c/opengl/learnopengl/00_window

Example from https://learnopengl.com/Getting-started/Hello-Window

In this project I learned how to:
- Use GLAD and GLFW to work together with OpenGL.
- Generate glad.h (https://glad.dav1d.de)
- Compile glad.c into a library (ar rcs libglad.a glad.o, then move libglad.a to /usr/lib)
- Link with glad as a gcc flag using -lglad.
- Create a window.
- Resize a window.

Compile and run with `make run`.
