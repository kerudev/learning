# c/opengl/learnopengl/00_window

Example from https://learnopengl.com/Getting-started/Hello-Window

In this project I learned how to:
- Use GLAD and GLFW to work together with OpenGL.
- Generate glad (https://glad.dav1d.de)
- Move glad.h to make it available globally
    - `sudo mv glad.h /usr/include/glad`
- Compile glad.c into a static library
    - `gcc glad.c -o glad.o`
    - `ar rcs libglad.a glad.o`
    - `sudo mv libglad.a /usr/lib`
- Link the with the glad library (`-lglad`).
- Create a window.
- Resize a window.

Compile and run with `make run`.
